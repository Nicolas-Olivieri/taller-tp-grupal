#ifndef SDL_H
#define SDL_H

#include <map>
#include <string>
#include <vector>

#include <SDL2/SDL_events.h>
#include <SDL2pp/Renderer.hh>
#include <SDL2pp/SDL2pp.hh>

#include "client/connection/connection_handler.h"
#include "common/queue.h"
#include "sprites/sprite.h"
#include "sprites/sprite_creator.h"
#include "sprites/texture_pool.h"

class ClientGame {
private:
    SDL2pp::SDL sdl;
    SDL2pp::Window window;
    SDL2pp::Renderer renderer;

    std::string player_name;
    TexturePool texture_pool;
    SpriteCreator sprite_creator;
    std::map<std::string, Sprite> players;

    ConnectionHandler& connection;

    int key_being_pressed;

public:
    ClientGame(ConnectionHandler& connection, std::string& player_name);

    void run();

private:
    // Principales
    int pollEvents();

    void update_state_from_server();

    void update_visuals(int it);

    void render_in_z_order();

    // Auxiliares
    void add_new_player(const PlayerInfoDTO& info);

    void handle_key_down(const SDL_Event& event);

    void handle_action(const ActionDTO& action);

    void update_players(const std::vector<PlayerInfoDTO>& players_information);
};


#endif
