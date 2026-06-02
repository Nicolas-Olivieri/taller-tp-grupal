#include "world.h"

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
            default:
                break;
        }
    }
}


void World::add_new_player(const PlayerInfoDTO& info) {
    Sprite user = sprite_creator.create_user(info);
    players.insert({{info.name, user}});
}


Sprite& World::get_client_player() { return players.at(player_name); }

SDL2pp::Rect& World::get_world_size() { return world_view; }
