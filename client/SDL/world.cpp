#include "world.h"

#include <unordered_set>
#include <vector>

#include "camera.h"

World::World(SDL2pp::Renderer& renderer, std::string& player_name):
        renderer(renderer),
        texture_pool(renderer),
        sprite_creator(renderer),
        world_view(SDL2pp::Point(0, 0), SDL2pp::Point(WORLD_WIDTH, WORLD_HEIGHT)),
        player_name(player_name) {}

void World::update_visuals(const int it) {
    for (auto& [name, entity]: players) {
        entity.update_visual_position();
        entity.update_frame(it);
    }

    for (auto& [sub_id, creature]: creatures) {
        creature.update_visual_position();
        creature.update_frame(it);
    }
}

void World::render_in_z_order(const Camera& camera) {
    //    Texture bg(renderer, DATA_PATH "/fondo2.jpg");
    //    renderer.Copy(bg, camera.get_view(), Rect(0, 0, WORLD_WIDTH, WORLD_HEIGHT));

    std::vector<Sprite> viewed_sprites = filter_viewed_sprites(camera);

    for (auto& entity: viewed_sprites) {
        entity.render(camera.get_view().GetTopLeft());
    }
    renderer.Present();
}

std::vector<Sprite> World::filter_viewed_sprites(const Camera& camera) {
    std::vector<Sprite> viewed_sprites;
    for (auto& [name, entity]: players) {
        if (entity.intersects(camera.get_view(), camera.get_view().GetTopLeft())) {
            viewed_sprites.push_back(entity);
        }
    }

    for (auto& [sub_id, creature]: creatures) {
        if (creature.intersects(camera.get_view(), camera.get_view().GetTopLeft())) {
            viewed_sprites.push_back(creature);
        }
    }

    return viewed_sprites;
}


void World::update_players(const std::vector<PlayerInfoDTO>& players_information) {
    for (const PlayerInfoDTO& player_info: players_information) {
        if (!players.contains(player_info.name)) {
            add_new_player(player_info);
        }

        SDL2pp::Point position(player_info.x, player_info.y);
        players.at(player_info.name).set_target_position(player_info.direction, position);
    }
}

void World::update_creatures(const std::vector<CreatureInfoDTO>& creatures_information) {
    erase_dead_creatures(creatures_information);

    for (const CreatureInfoDTO& creature_info: creatures_information) {
        if (!creatures.contains(creature_info.sub_id)) {
            add_new_creature(creature_info);
        }

        SDL2pp::Point position(creature_info.x, creature_info.y);
        creatures.at(creature_info.sub_id).set_target_position(creature_info.direction, position);
    }
}

void World::erase_dead_creatures(const std::vector<CreatureInfoDTO>& creatures_information) {
    std::unordered_set<uint16_t> sub_ids;
    for (const auto& info: creatures_information) {
        sub_ids.insert(info.sub_id);
    }

    for (auto it = creatures.begin(); it != creatures.end();) {
        if (!sub_ids.contains(it->first)) {
            it = creatures.erase(it);
        } else {
            it++;
        }
    }
}


void World::handle_actions(const std::vector<ActionDTO>& actions) {
    // TODO agregar todos los tipos que vayamos agregando
    for (auto& action: actions) {
        switch (action.action) {
            case ActionType::DESPAWN:
                if (players.contains(action.despawn.player_despawned)) {
                    players.extract(action.despawn.player_despawned);
                }
                break;
            case ActionType::RESURRECTION:
                if (players.contains(action.resurrection.player_resurrected)) {
                    Sprite& sprite = players.at(action.resurrection.player_resurrected);
                    sprite_creator.update_appearance(sprite, action.resurrection.original_appearance);
                }
                break;
            case ActionType::DEATH:
                if (players.contains(action.death.player_dead)) {
                    Sprite& sprite = players.at(action.death.player_dead);
                    sprite_creator.convert_to_ghost(sprite);
                }
                break;
            default:
                break;
        }
    }
}


void World::add_new_player(const PlayerInfoDTO& info) {
    Sprite user = sprite_creator.create_user(info);
    players.insert({{info.name, user}});
}

void World::add_new_creature(const CreatureInfoDTO& info) {
    Sprite creature = sprite_creator.create_creature(info);
    creatures.insert({{info.sub_id, creature}});
}

Sprite& World::get_client_player() { return players.at(player_name); }

SDL2pp::Rect& World::get_world_size() { return world_view; }
