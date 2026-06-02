#ifndef CLIENT_GAME_H
#define CLIENT_GAME_H

#include <string>
#include <vector>

#include <SDL2/SDL_events.h>
#include <SDL2pp/Renderer.hh>
#include <SDL2pp/SDL2pp.hh>

#include "client/connection/connection_handler.h"

#include "camera.h"
#include "user_interface.h"
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
    const SDL2pp::Rect game_viewport = {10, 220, 735, 500};
    Camera camera;

    UserInterface ui;

    bool is_chat_active;
    std::string chat_text;
    const SDL2pp::Rect chat_icon = {5, 120, 45, 30};

    // Principales
    int pollEvents();

    void render_ui_and_world();

    void update_state_from_server();

    void handle_key_down(const SDL_Event& event);

    void handle_mouse_click(const SDL_Event& event);

    void handle_ui_click(const SDL_Event& event);

    void handle_game_click(const SDL_Event& event);

    bool is_inside_viewport(int x, int y, const SDL2pp::Rect& viewport);

    void toggle_chat();

    void handle_chat_events(const SDL_Event& event);

    void send_private_message();

public:
    ClientGame(ConnectionHandler& connection, std::string& player_name);

    void run();

    Camera initialize_world_and_camera();
};


#endif  // CLIENT_GAME_H
