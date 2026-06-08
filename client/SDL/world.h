#ifndef WORLD_H
#define WORLD_H

#include <map>
#include <memory>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "common/dto/snapshot/actions/action.h"
#include "common/dto/snapshot/map/client_map_data.h"
#include "sprites/sprite_creator.h"
#include "sprites/texture_pool.h"

#include "camera.h"

class World {
private:
    SDL2pp::Renderer& renderer;
    TexturePool texture_pool;
    SpriteCreator sprite_creator;

    SDL2pp::Rect world_view;
    std::string player_name;
    std::map<std::string, std::shared_ptr<Sprite>> players;
    std::map<uint16_t, std::shared_ptr<Sprite>> creatures;
    std::map<std::pair<uint16_t, uint16_t>, std::pair<std::shared_ptr<Sprite>, bool>> loot;

    std::set<std::shared_ptr<Sprite>> map_tiles;
    std::set<std::shared_ptr<Sprite>> map_loot;
    std::set<std::shared_ptr<Sprite>> map_items;

    void init_assets(const ClientMapDataDTO& map_data);


    void add_new_player(const PlayerInfoDTO& info);

    static bool cmp_by_y_coord(const std::shared_ptr<Sprite>& a, const std::shared_ptr<Sprite>& b);

    void add_new_creature(const CreatureInfoDTO& info);

    void add_new_loot(const LootInfoDTO& info, const std::pair<uint16_t, uint16_t>& place);

    void erase_dead_creatures(const std::vector<CreatureInfoDTO>& creatures_information);

    void erase_taken_loot(const std::vector<LootInfoDTO>& loot_information);

public:
    World(SDL2pp::Renderer& renderer, const ClientMapDataDTO& map_data, std::string& player_name);

    void update_players(const std::vector<PlayerInfoDTO>& players_information);

    void update_creatures(const std::vector<CreatureInfoDTO>& creatures_information);

    void update_loot(const std::vector<LootInfoDTO>& loot_information);

    void handle_actions(const std::vector<ActionDTO>& actions);

    void update_visuals(int iteration);

    void render_in_z_order(const Camera& camera) const;

    std::vector<std::shared_ptr<Sprite>> filter_viewed_sprites(
            const Camera& camera, const std::set<std::shared_ptr<Sprite>>& sprites) const;

    Sprite& get_client_player();

    SDL2pp::Rect& get_world_size();
};


#endif  // WORLD_H
