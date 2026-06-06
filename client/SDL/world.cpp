#include "world.h"

#include <vector>

#include "camera.h"

World::World(SDL2pp::Renderer& renderer, const ClientMapDataDTO& map_data, std::string& player_name):
        renderer(renderer),
        texture_pool(renderer),
        sprite_creator(renderer),
        world_view(SDL2pp::Point(0, 0), SDL2pp::Point(map_data.world_width*TILE_SIZE, map_data.world_height*TILE_SIZE)),
        player_name(player_name) {
    init_assets(map_data);
}

void World::init_assets(const ClientMapDataDTO& map_data) {
    for (const auto& tile_data : map_data.tiles) {
        Sprite tile = sprite_creator.create_asset("tiles", tile_data);
        map_tiles.emplace(std::make_shared<Sprite>(tile));
    }

    for (const auto& collider_data : map_data.colliders) {
        Sprite collider = sprite_creator.create_asset("colliders", collider_data);
        map_items.emplace(std::make_shared<Sprite>(collider));
    }

    for (const auto& npc_data : map_data.npcs) {
        Sprite npc = sprite_creator.create_asset("npcs", npc_data);
        map_items.emplace(std::make_shared<Sprite>(npc));
    }
}



void World::update_visuals(const int it) {
    for (auto& [name, entity]: players) {
        entity->update_visual_position();
        entity->update_frame(it);
    }
}

bool World::cmp_by_y_coord(const std::shared_ptr<Sprite>& a, const std::shared_ptr<Sprite>& b) {
    return a->get_ground_position().y < b->get_ground_position().y;
}

void World::render_in_z_order(const Camera& camera) const {
    // Obtengo tiles e items (colliders, npcs, players, enemies) que se llegan a ver en la cámara
    auto viewed_tiles = filter_viewed_sprites(camera, map_tiles);
    auto viewed_items = filter_viewed_sprites(camera, map_items);

    // Ordeno los items por y
    std::ranges::sort(viewed_items, cmp_by_y_coord);

    // Renderizo primero los tiles y luego los items por encima
    for (const auto& tile: viewed_tiles) {
        tile->render(camera.get_view().GetTopLeft());
    }
    for (const auto& item : viewed_items) {
        item->render(camera.get_view().GetTopLeft());
    }
    renderer.Present();
}

std::vector<std::shared_ptr<Sprite>> World::filter_viewed_sprites(const Camera &camera, const std::set<std::shared_ptr<Sprite>> &sprites) const {
    std::vector<std::shared_ptr<Sprite>> viewed_sprites;

    for (auto& item : sprites) {
        if (item->intersects(camera.get_view(), camera.get_view().GetTopLeft())) {
            viewed_sprites.push_back(item);
        }
    }
    return viewed_sprites;
}


void World::update_players(const std::vector<PlayerInfoDTO>& players_information) {
    for (const PlayerInfoDTO& player_info: players_information) {
        if (!players.contains(player_info.name)) {
            add_new_player(player_info);
        }

        // Se le resta 1 para que los jugadores se impriman "una celda para arriba" y se alinee con el mapa
        SDL2pp::Point position(player_info.x, player_info.y-1);
        players.at(player_info.name)->set_target_position(player_info.direction, position);
    }
}

void World::handle_actions(const std::vector<ActionDTO>& actions) {
    // TODO agregar todos los tipos que vayamos agregando
    for (auto& action: actions) {
        switch (action.action) {
            case ActionType::DESPAWN:
                if (players.contains(action.despawn.player_despawned)) {
                    auto player = players.extract(action.despawn.player_despawned);
                    map_items.erase(player.mapped());
                }
                break;
            case ActionType::RESURRECTION:
                if (players.contains(action.resurrection.player_resurrected)) {
                    Sprite* sprite = players.at(action.resurrection.player_resurrected).get();
                    sprite_creator.update_appearance(*sprite, action.resurrection.original_appearance);
                }
                break;
            case ActionType::DEATH:
                if (players.contains(action.death.player_dead)) {
                    Sprite* sprite = players.at(action.death.player_dead).get();
                    sprite_creator.convert_to_ghost(*sprite);
                }
                break;
            default:
                break;
        }
    }
}


void World::add_new_player(const PlayerInfoDTO& info) {
    Sprite player = sprite_creator.create_user(info);
    std::shared_ptr<Sprite> ptr = std::make_shared<Sprite>(player);
    players.insert({{info.name, ptr}});
    map_items.emplace(ptr);
}


Sprite& World::get_client_player() { return *players.at(player_name).get(); }

SDL2pp::Rect& World::get_world_size() { return world_view; }
