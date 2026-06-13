#include "world.h"

#include <algorithm>
#include <memory>
#include <unordered_set>
#include <vector>

#include "camera.h"

World::World(SDL2pp::Renderer& renderer, const ClientMapDataDTO& map_data, std::string& player_name,
             AudioManager& audio_manager):
        renderer(renderer),
        texture_pool(renderer),
        sprite_creator(renderer),
        audio_manager(audio_manager),
        world_view(SDL2pp::Point(0, 0),
                   SDL2pp::Point(map_data.world_width * TILE_SIZE, map_data.world_height * TILE_SIZE)),
        player_name(player_name) {
    init_assets(map_data);
}

void World::init_assets(const ClientMapDataDTO& map_data) {
    for (const auto& tile_data: map_data.tiles) {
        FixedSprite tile = sprite_creator.create_sprite(SpriteCategory::TILE, tile_data);
        map_tiles.emplace(std::make_shared<FixedSprite>(tile));
    }

    for (const auto& collider_data: map_data.colliders) {
        FixedSprite collider = sprite_creator.create_sprite(SpriteCategory::COLLIDER, collider_data);
        map_items.emplace(std::make_shared<FixedSprite>(collider));
    }

    for (const auto& npc_data: map_data.npcs) {
        FixedSprite npc = sprite_creator.create_sprite(SpriteCategory::NPC, npc_data);
        map_items.emplace(std::make_shared<FixedSprite>(npc));
    }
}


void World::update_visuals(const int it) const {
    for (auto& tile: map_tiles) {
        tile->update_frame(it);
    }
    for (auto& entity: map_entities) {
        entity->update_visual_position();
    }
    for (auto& item: map_items) {
        item->update_frame(it);
    }
    for (auto& fx: effects) {
        fx->update_frame(it);
    }
}

bool World::cmp_by_y_coord(const std::shared_ptr<Sprite>& a, const std::shared_ptr<Sprite>& b) {
    return a->get_ground_position().y <= b->get_ground_position().y;
}

void World::render_in_z_order(const Camera& camera) const {
    // Obtengo tiles e items (colliders, npcs, players, enemies) que se llegan a ver en la cámara
    auto viewed_tiles = filter_viewed_sprites(camera, map_tiles);
    auto viewed_loot = filter_viewed_sprites(camera, map_loot);
    auto viewed_items = filter_viewed_sprites(camera, map_items);
    auto viewed_effects = filter_viewed_sprites(camera, effects);

    // Ordeno los items por y
    std::ranges::stable_sort(viewed_items, cmp_by_y_coord);

    // Renderizo primero los tiles y luego los loot/items/fx por encima
    for (const auto& tile: viewed_tiles) {
        tile->render(camera.get_view().GetTopLeft());
    }

    for (const auto& loot_item: viewed_loot) {
        loot_item->render(camera.get_view().GetTopLeft());
    }

    for (const auto& item: viewed_items) {
        item->render(camera.get_view().GetTopLeft());
    }

    for (const auto& fx: viewed_effects) {
        fx->render(camera.get_view().GetTopLeft());
    }
}

template <typename Container>
std::vector<std::shared_ptr<Sprite>> World::filter_viewed_sprites(const Camera& camera,
                                                                  const Container& sprites) const {
    std::vector<std::shared_ptr<Sprite>> viewed_sprites;

    std::ranges::copy_if(sprites, std::back_inserter(viewed_sprites), [camera](auto& item) {
        return item->intersects(camera.get_view(), camera.get_view().GetTopLeft());
    });

    return viewed_sprites;
}


void World::update_players(const std::vector<PlayerInfoDTO>& players_information) {
    for (const PlayerInfoDTO& player_info: players_information) {
        if (!players.contains(player_info.name)) {
            add_new_player(player_info);
            audio_manager.play_event(SoundEvent::SPAWN);
        }

        SDL2pp::Point position(player_info.x, player_info.y);
        const auto& player_sprite = players.at(player_info.name);
        if (position * TILE_SIZE != player_sprite->get_target_position())
            audio_manager.play_event(SoundEvent::FOOTSTEP);

        player_sprite->set_target_position(player_info.direction, position);

        sprite_creator.update_appearance(*player_sprite, player_info.appearance, player_info.equipment);
    }
}

void World::update_creatures(const std::vector<CreatureInfoDTO>& creatures_information) {
    erase_dead_creatures(creatures_information);

    for (const CreatureInfoDTO& creature_info: creatures_information) {
        if (!creatures.contains(creature_info.sub_id)) {
            add_new_creature(creature_info);
            audio_manager.play_event(SoundEvent::SPAWN);
        }

        SDL2pp::Point position(creature_info.x, creature_info.y);
        const auto& creature_sprite = creatures.at(creature_info.sub_id);
        if (position * TILE_SIZE != creature_sprite->get_target_position())
            audio_manager.play_event(SoundEvent::FOOTSTEP);

        creature_sprite->set_target_position(creature_info.direction, position);
    }
}

void World::erase_dead_creatures(const std::vector<CreatureInfoDTO>& creatures_information) {
    std::unordered_set<uint16_t> sub_ids;
    for (const auto& info: creatures_information) {
        sub_ids.insert(info.sub_id);
    }

    for (auto it = creatures.begin(); it != creatures.end();) {
        if (!sub_ids.contains(it->first)) {
            map_items.erase(it->second);
            map_entities.erase(it->second);
            it = creatures.erase(it);
            audio_manager.play_event(SoundEvent::DEATH);
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
        } else if (loot.at(place).second != loot_info.is_item) {
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
                    map_items.erase(player.mapped());
                    map_entities.erase(player.mapped());
                    audio_manager.play_event(SoundEvent::DESPAWN);
                }
                break;

            case ActionType::HEAL:
                audio_manager.play_event(SoundEvent::HEAL);
                break;

            case ActionType::RESURRECTION:
                if (players.contains(action.resurrection.player_resurrected)) {
                    PlayerSprite* sprite = players.at(action.resurrection.player_resurrected).get();
                    sprite_creator.update_appearance(*sprite, action.resurrection.original_appearance);
                    audio_manager.play_event(SoundEvent::RESURRECTION);
                }
                break;
            case ActionType::DEATH:
                if (players.contains(action.death.player_dead)) {
                    PlayerSprite* sprite = players.at(action.death.player_dead).get();
                    sprite_creator.convert_to_ghost(*sprite);

                    const EffectSprite fx = sprite_creator.create_sprite(action, sprite->get_position());
                    auto ptr = std::make_shared<EffectSprite>(fx);
                    effects.emplace(ptr);

                    audio_manager.play_event(SoundEvent::DEATH);
                }
                break;

            case ActionType::ATTACK: {
                // TODO: Cambiar el SFX según el arma con la que se atacó
                audio_manager.play_event(SoundEvent::SWORD_ATTACK);

                if (!action.attack.missed) {
                    const EffectSprite fx = sprite_creator.create_sprite(action);
                    auto ptr = std::make_shared<EffectSprite>(fx);
                    effects.emplace(ptr);
                }

                break;
            }

            default:
                break;
        }
    }
}


void World::add_new_player(const PlayerInfoDTO& info) {
    PlayerSprite player = sprite_creator.create_sprite(info);
    auto ptr = std::make_shared<PlayerSprite>(player);
    players.insert({{info.name, ptr}});
    map_items.emplace(ptr);
    map_entities.emplace(ptr);
}

void World::add_new_creature(const CreatureInfoDTO& info) {
    EnemySprite creature = sprite_creator.create_sprite(info);
    auto ptr = std::make_shared<EnemySprite>(creature);
    creatures.insert({{info.sub_id, ptr}});
    map_items.emplace(ptr);
    map_entities.emplace(ptr);
}

void World::add_new_loot(const LootInfoDTO& info, const std::pair<uint16_t, uint16_t>& place) {
    FixedSprite drop = sprite_creator.create_sprite(info);
    auto ptr = std::make_shared<FixedSprite>(drop);
    loot[place] = {ptr, info.is_item};
    map_loot.emplace(ptr);
}

void World::update_top_loot(const LootInfoDTO& info, const std::pair<uint16_t, uint16_t>& place) {
    auto& [sprite, is_item] = loot[place];
    map_loot.extract(sprite);
    add_new_loot(info, place);
}

PlayerSprite& World::get_client_player() { return *players.at(player_name).get(); }

SDL2pp::Rect& World::get_world_size() { return world_view; }
