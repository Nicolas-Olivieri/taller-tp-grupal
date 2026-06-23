#include "world.h"

#include <algorithm>
#include <cmath>
#include <memory>
#include <ranges>
#include <unordered_set>
#include <vector>

#include "client/config/client_config.h"
#include "client/util/sdl_grid_range.h"
#include "camera.h"

#define TILE_IDX 0
#define SAFE_ZONE_IDX 1
#define LOOT_IDX 2
#define ITEM_IDX 3
#define COLLIDER_IDX 4

World::World(SpriteCreator& sprite_creator, const ClientMapDataDTO& map_data, const std::string& player_name,
             AudioManager& audio_manager):
        sprite_creator(sprite_creator),
        audio_manager(audio_manager),
        world_view(SDL2pp::Point(0, 0),
                   SDL2pp::Point(map_data.world_width * ClientConfig::get().get_tile_size(),
                                 map_data.world_height * ClientConfig::get().get_tile_size())),
        player_name(player_name) {
    init_assets(map_data);
}

void World::init_assets(const ClientMapDataDTO& map_data) {
    fixed_items.assign(map_data.world_height+1, {});
    for (auto& row : fixed_items) {
        row.assign(map_data.world_width+1, CellSprites{});
    }

    for (const auto& tile_data: map_data.tiles) {
        FixedSprite tile = sprite_creator.create_sprite(SpriteCategory::TILE, tile_data);
        SDLGridRange range(tile_data.x, tile_data.y, tile.get_size()/ClientConfig::get().get_tile_size());
        auto ptr = std::make_shared<FixedSprite>(std::move(tile));

        for (const auto& cell : range) {
            auto& cell_sprites = fixed_items.at(cell.y).at(cell.x).sprites;
            cell_sprites.assign(COLLIDER_IDX+1, nullptr);
            cell_sprites.at(TILE_IDX) = ptr;
        }
    }

    for (const auto& safe_zone_data: map_data.safe_zones) {
        FixedSprite safe_zone = sprite_creator.create_sprite(SpriteCategory::SAFE_ZONE, safe_zone_data);
        auto ptr = std::make_shared<FixedSprite>(std::move(safe_zone));

        if (fixed_items.at(safe_zone_data.y).at(safe_zone_data.x).sprites.empty()) continue;
        auto& cell_sprites = fixed_items.at(safe_zone_data.y).at(safe_zone_data.x).sprites;
        cell_sprites.at(SAFE_ZONE_IDX) = ptr;
    }

    for (const auto& collider_data: map_data.colliders) {
        FixedSprite collider = sprite_creator.create_sprite(SpriteCategory::COLLIDER, collider_data);
        SDLGridRange range(collider_data.x, collider_data.y,
                       collider.get_size()/ClientConfig::get().get_tile_size());
        auto ptr = std::make_shared<FixedSprite>(std::move(collider));

        for (const auto& cell : range) {
            auto& cell_sprites = fixed_items.at(cell.y).at(cell.x).sprites;
            cell_sprites.at(COLLIDER_IDX) = ptr;
        }
    }

    for (const auto& npc_data: map_data.npcs) {
        FixedSprite npc = sprite_creator.create_sprite(SpriteCategory::NPC, npc_data);
        SDLGridRange range(npc_data.x, npc_data.y, npc.get_size()/ClientConfig::get().get_tile_size());
        auto ptr = std::make_shared<FixedSprite>(std::move(npc));

        for (const auto& cell : range) {
            auto& cell_sprites = fixed_items.at(cell.y).at(cell.x).sprites;
            cell_sprites.at(COLLIDER_IDX) = ptr;
        }
    }
}


void World::update_visuals() const {
    for (auto& entity: map_entities) {
        entity->update_visual_position();
    }
}

bool World::cmp_by_y_coord(const std::shared_ptr<WorldSprite>& a, const std::shared_ptr<WorldSprite>& b) {
    return a->get_ground_position().y <= b->get_ground_position().y;
}

void World::render_in_z_order(const Camera& camera, int iteration) const {
    auto viewed_effects = filter_viewed_sprites(camera, effects);
    std::vector<std::shared_ptr<WorldSprite>> viewed_items;


    auto is_visible = [&camera](const auto& item) {
        return item->intersects(camera.get_view(), camera.get_view().GetTopLeft());
    };
    for (auto& entity : map_entities) {
        if (is_visible(entity)) {
            viewed_items.push_back(entity);
        }
    }

    const uint8_t tile_size = ClientConfig::get().get_tile_size();
    const SDL2pp::Rect camera_view = camera.get_view();
    SDL2pp::Point origin = camera_view.GetTopLeft() / tile_size;
    SDL2pp::Point size = camera_view.GetSize() / tile_size + SDL2pp::Point(3,3);

    SDLGridRange viewed_range(origin, size);
    for (const auto& cell : viewed_range) {
        auto cell_sprites = fixed_items.at(cell.y).at(cell.x).sprites;
        for (size_t i = 0; i <= ITEM_IDX; i++) {
            if (cell_sprites.size() < i ||
                cell_sprites.at(i) == nullptr ||
                cell_sprites.at(i)->already_selected_for_frame(iteration)) continue;

            cell_sprites.at(i)->update_frame(iteration);
            cell_sprites.at(i)->render(camera.get_view().GetTopLeft());
            cell_sprites.at(i)->set_last_frame(iteration);
        }

        if (cell_sprites.size() < COLLIDER_IDX ||
            cell_sprites.at(COLLIDER_IDX) == nullptr ||
            cell_sprites.at(COLLIDER_IDX)->already_selected_for_frame(iteration)) continue;
        viewed_items.push_back(cell_sprites.at(COLLIDER_IDX));
        cell_sprites.at(COLLIDER_IDX)->set_last_frame(iteration);
    }


    // Ordeno los items por y
    std::ranges::stable_sort(viewed_items, cmp_by_y_coord);

    for (const auto& item: viewed_items) {
        item->update_frame(iteration);
        item->render(camera.get_view().GetTopLeft());
    }

    for (const auto& fx: viewed_effects) {
        fx->update_frame(iteration);
        fx->render(camera.get_view().GetTopLeft());
    }
}

void World::update_players(const std::vector<PlayerInfoDTO>& players_information) {
    const uint16_t tile_size = ClientConfig::get().get_tile_size();

    for (const PlayerInfoDTO& player_info: players_information) {
        if (!players.contains(player_info.name)) {
            add_new_player(player_info);
            play_event(SoundEvent::SPAWN, SDL2pp::Point(player_info.x, player_info.y) * tile_size);
        }

        SDL2pp::Point position(player_info.x, player_info.y);
        const auto& player_sprite = players.at(player_info.name);
        if (position * tile_size != player_sprite->get_target_position())
            play_event(SoundEvent::FOOTSTEP, SDL2pp::Point(player_info.x, player_info.y) * tile_size);

        player_sprite->set_target_position(player_info.direction, position);

        sprite_creator.update_label(*player_sprite, player_info);

        sprite_creator.update_appearance(*player_sprite, player_info.appearance, player_info.equipment);
    }
}

void World::update_creatures(const std::vector<CreatureInfoDTO>& creatures_information) {
    const uint16_t tile_size = ClientConfig::get().get_tile_size();

    erase_dead_creatures(creatures_information);

    for (const CreatureInfoDTO& creature_info: creatures_information) {
        if (!creatures.contains(creature_info.sub_id)) {
            add_new_creature(creature_info);
            play_event(SoundEvent::SPAWN, SDL2pp::Point(creature_info.x, creature_info.y) * tile_size);
        }

        SDL2pp::Point position(creature_info.x, creature_info.y);
        const auto& creature_sprite = creatures.at(creature_info.sub_id);
        if (position * tile_size != creature_sprite->get_target_position())
            play_event(SoundEvent::FOOTSTEP, SDL2pp::Point(creature_info.x, creature_info.y) * tile_size);

        creature_sprite->set_target_position(creature_info.direction, position);

        sprite_creator.update_label(*creature_sprite, creature_info);
    }
}

void World::erase_dead_creatures(const std::vector<CreatureInfoDTO>& creatures_information) {
    std::unordered_set<uint16_t> sub_ids;
    for (const auto& info: creatures_information) {
        sub_ids.insert(info.sub_id);
    }

    for (auto it = creatures.begin(); it != creatures.end();) {
        if (!sub_ids.contains(it->first)) {
            play_event(SoundEvent::DEATH, it->second.get()->get_position());
            map_items.erase(it->second);
            map_entities.erase(it->second);
            it = creatures.erase(it);
        } else {
            it++;
        }
    }
}

void World::update_loot(const std::vector<LootInfoDTO>& loot_information) {
    erase_taken_loot(loot_information);

    for (const LootInfoDTO& loot_info: loot_information) {
        const std::pair<uint16_t, uint16_t> place = {loot_info.x, loot_info.y};
        if (!loot.contains(place)) {
            add_new_loot(loot_info, place);
        } else if (loot.at(place).second != loot_info.type) {
            update_top_loot(loot_info, place);
        }
    }
}

void World::erase_taken_loot(const std::vector<LootInfoDTO>& loot_information) {
    // TODO: este capaz si se puede cambiar por un ActionDTO que broadcastea PickUpCommand; es un poco más
    // costoso por ser un map ordenado
    std::set<std::pair<uint16_t, uint16_t>> places;
    for (const auto& info: loot_information) {
        places.insert({info.x, info.y});
    }

    for (auto it = loot.begin(); it != loot.end();) {
        if (!places.contains(it->first)) {
            SDL2pp::Point cell = it->second.first->get_position() / ClientConfig::get().get_tile_size();
            auto& cell_items = fixed_items.at(cell.y).at(cell.x).sprites;
            cell_items.insert(cell_items.begin()+LOOT_IDX, nullptr);

            map_loot.erase(it->second.first);
            it = loot.erase(it);
        } else {
            it++;
        }
    }
}

void World::erase_finished_effects() {
    std::erase_if(effects, [](const std::shared_ptr<EffectSprite>& fx) { return fx->has_finished(); });
}

void World::handle_actions(const std::vector<ActionDTO>& actions) {
    // TODO agregar todos los tipos que vayamos agregando
    for (auto& action: actions) {
        switch (action.action) {
            case ActionType::DESPAWN:
                if (players.contains(action.despawn.player_despawned)) {
                    auto player = players.extract(action.despawn.player_despawned);
                    play_event(SoundEvent::DESPAWN, player.mapped()->get_position());
                    map_items.erase(player.mapped());
                    map_entities.erase(player.mapped());
                }
                break;

            case ActionType::HEAL:
                if (players.contains(action.heal.player_healed)) {
                    const Sprite* sprite = players.at(action.heal.player_healed).get();
                    play_event(SoundEvent::HEAL, sprite->get_position());
                }
                break;

            case ActionType::MEDITATION:
                if (players.contains(action.meditation.player_meditating)) {
                    const Sprite* sprite = players.at(action.meditation.player_meditating).get();

                    EffectSprite fx = sprite_creator.create_sprite(action, sprite->get_position());
                    auto ptr = std::make_shared<EffectSprite>(std::move(fx));
                    ptr.get()->set_visual_position(sprite->get_position() -
                                                   SDL2pp::Point(0, sprite->get_size().GetY() / 2));
                    effects.emplace(ptr);

                    play_event(SoundEvent::MEDITATION, sprite->get_position());
                }
                break;

            case ActionType::RESURRECTION:
                if (players.contains(action.resurrection.player_resurrected)) {
                    PlayerSprite* sprite = players.at(action.resurrection.player_resurrected).get();
                    sprite_creator.update_appearance(*sprite, action.resurrection.original_appearance);
                    play_event(SoundEvent::RESURRECTION, sprite->get_position());
                }
                break;

            case ActionType::DEATH:
                if (players.contains(action.death.player_dead)) {
                    PlayerSprite* sprite = players.at(action.death.player_dead).get();
                    sprite_creator.convert_to_ghost(*sprite);

                    EffectSprite fx = sprite_creator.create_sprite(action, sprite->get_position());
                    auto ptr = std::make_shared<EffectSprite>(std::move(fx));
                    ptr.get()->set_visual_position(sprite->get_position() -
                                                   SDL2pp::Point(0, sprite->get_size().GetY() / 2));
                    effects.emplace(ptr);

                    play_event(SoundEvent::DEATH, sprite->get_position());
                }
                break;

            case ActionType::ATTACK:
                handle_attack(action.attack);

                if (!action.attack.missed) {
                    EffectSprite fx = sprite_creator.create_sprite(action);
                    auto ptr = std::make_shared<EffectSprite>(std::move(fx));
                    effects.emplace(ptr);
                }

                break;

            case ActionType::CLAN_ACCEPT:
                if (players.contains(action.clan_accept.founder)) {
                    const Sprite* sprite = players.at(action.clan_accept.founder).get();
                    play_event(SoundEvent::CLAN_ACCEPT, sprite->get_position());
                }
                if (players.contains(action.clan_accept.accepted)) {
                    const Sprite* sprite = players.at(action.clan_accept.accepted).get();
                    play_event(SoundEvent::CLAN_ACCEPT, sprite->get_position());
                }
                break;


            case ActionType::CLAN_FOUND:
                if (players.contains(action.clan_found.founder)) {
                    const Sprite* sprite = players.at(action.clan_found.founder).get();
                    play_event(SoundEvent::CLAN_FOUND, sprite->get_position());
                }
                break;

            case ActionType::CLAN_LEAVE:
                if (players.contains(action.clan_leave.leaver)) {
                    const Sprite* sprite = players.at(action.clan_leave.leaver).get();
                    play_event(SoundEvent::CLAN_LEAVE, sprite->get_position());
                }
                break;

            default:
                break;
        }
    }
}

void World::handle_attack(const AttackDTO& attack) {
    // TODO: Este mapa debería estar en otro lugar (o que el SoundEvent sea un atributo de un ítem en
    //  ClientConfig)
    static const std::map<uint8_t, SoundEvent> weapon_to_sound_event{
            {0, SoundEvent::FISTS_ATTACK},      {1, SoundEvent::SWORD_ATTACK},
            {2, SoundEvent::AXE_ATTACK},        {3, SoundEvent::HAMMER_ATTACK},
            {4, SoundEvent::MAGIC_ARROW_SPELL}, {5, SoundEvent::HEAL_SPELL},
            {6, SoundEvent::MISSILE_SPELL},     {7, SoundEvent::EXPLOSION_SPELL},
            {8, SoundEvent::BOW_ATTACK},        {9, SoundEvent::BOW_ATTACK},
            {20, SoundEvent::SWORD_ATTACK},     {21, SoundEvent::DISTORTION_SPELL},
            {22, SoundEvent::BOW_ATTACK}};

    if (not weapon_to_sound_event.contains(attack.weapon))
        return;

    play_event(weapon_to_sound_event.at(attack.weapon),
               SDL2pp::Point(attack.x, attack.y) * ClientConfig::get().get_tile_size());
}

void World::add_new_player(const PlayerInfoDTO& info, const bool is_client_player) {
    PlayerSprite player = sprite_creator.create_sprite(info, is_client_player);
    auto ptr = std::make_shared<PlayerSprite>(std::move(player));
    players.insert({{info.name, ptr}});
    map_items.emplace(ptr);
    map_entities.emplace(ptr);
}

void World::add_new_creature(const CreatureInfoDTO& info) {
    EnemySprite creature = sprite_creator.create_sprite(info);
    auto ptr = std::make_shared<EnemySprite>(std::move(creature));
    creatures.insert({{info.sub_id, ptr}});
    map_items.emplace(ptr);
    map_entities.emplace(ptr);
}

void World::add_new_loot(const LootInfoDTO& info, const std::pair<uint16_t, uint16_t>& place) {
    FixedSprite drop = sprite_creator.create_sprite(info);
    auto ptr = std::make_shared<FixedSprite>(std::move(drop));
    loot[place] = {ptr, info.type};

    auto& cell_items = fixed_items.at(info.y).at(info.x).sprites;
    cell_items.insert(cell_items.begin()+LOOT_IDX, ptr);
}

void World::update_top_loot(const LootInfoDTO& info, const std::pair<uint16_t, uint16_t>& place) {
    auto& [sprite, type] = loot[place];
    map_loot.extract(sprite);
    add_new_loot(info, place);
}

PlayerSprite& World::get_client_player() {
    assert(players.contains(player_name));
    return *(players.at(player_name).get());
}

SDL2pp::Rect& World::get_world_size() { return world_view; }

void World::play_event(const SoundEvent& event, const SDL2pp::Point& source) {
    const auto& config = ClientConfig::get();
    const uint16_t tile_size = config.get_tile_size();
    const uint8_t max_distance = config.get_sound_data().max_sound_distance;

    const SDL2pp::Point listener = get_client_player().get_position();

    const int dx = source.x - listener.x;
    const int dy = source.y - listener.y;
    const double distance = std::sqrt(dx * dx + dy * dy);

    const double limit = max_distance * tile_size;
    if (distance >= limit)
        return;

    // TODO: Como idea, se podría multiplicar también por un factor aleatorio para que el sonido
    //  se escuche más o menos fuerte (entre un 10% más y un 10% menos, por ejemplo)

    audio_manager.play_event(event, 1.0 - distance / limit);
}
