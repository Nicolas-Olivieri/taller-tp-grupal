#ifndef CLIENT_GAME_H
#define CLIENT_GAME_H

#include <string>
#include <vector>

#include <SDL2/SDL_events.h>
#include <SDL2pp/Renderer.hh>
#include <SDL2pp/SDL2pp.hh>

#include "client/connection/connection_handler.h"

#include "camera.h"
#include "world.h"

class ClientGame {
private:
    SDL2pp::SDL sdl;
    SDL2pp::Window window;
    SDL2pp::Renderer renderer;
    ConnectionHandler& connection;

    std::string player_name;
    World world;
    int key_being_pressed;
    Camera camera;

    // Principales
    int pollEvents();

    void update_state_from_server();

    void handle_key_down(const SDL_Event& event);

    void handle_mouse_click(const SDL_Event& event);

public:
    ClientGame(ConnectionHandler& connection, std::string& player_name);

    void run();

    Camera initialize_world_and_camera();
};


#endif  // CLIENT_GAME_H
