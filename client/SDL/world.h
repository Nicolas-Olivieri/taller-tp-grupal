#ifndef WORLD_H
#define WORLD_H

#include <map>
#include <string>
#include <vector>

#include "common/dto/snapshot/actions/action.h"
#include "common/dto/snapshot/info/playerinfo.h"
#include "common/dto/snapshot/snapshot.h"
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
    std::map<std::string, Sprite> players;

    void add_new_player(const PlayerInfoDTO& info);

public:
    World(SDL2pp::Renderer& renderer, std::string& player_name);

    void update_players(const std::vector<PlayerInfoDTO>& players_information);
    void handle_actions(const std::vector<ActionDTO>& actions);

    void update_visuals(int iteration);

    void render_in_z_order(const Camera& camera);

    std::vector<Sprite> filter_viewed_sprites(const Camera& camera);

    Sprite& get_client_player();

    SDL2pp::Rect& get_world_size();
};


#endif  // WORLD_H
