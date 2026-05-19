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

    TexturePool texture_pool;
    SpriteCreator sprite_creator;
    std::map<std::string, Sprite> players;

    ConnectionHandler& connection;

public:
    explicit ClientGame(ConnectionHandler& connection);

    void run();

    int pollEvents();

    int pollEvents(Sprite& user) const;

    void update_state_from_server();

    void update_animation_frames(int it);

    void render_in_z_order();

private:
    void add_new_player(const AppearanceDTO& appearance);

    void handle_key_down(const SDL_Event& event);

    void handle_action(const ActionDTO& action);

    void update_players(const std::vector<PlayerInfoDTO>& players_information);
};


#endif
