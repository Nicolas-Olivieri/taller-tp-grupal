#ifndef CLIENT_GAME_H
#define CLIENT_GAME_H

#include <string>
#include <vector>

#include <SDL2/SDL_events.h>
#include <SDL2pp/Renderer.hh>
#include <SDL2pp/SDL2pp.hh>

#include "audio/audio_manager.h"
#include "client/connection/connection_handler.h"
#include "client/util/command_handler.h"
#include "client/util/key_handler.h"

#include "camera.h"
#include "user_interface.h"
#include "world.h"

class ClientGame {
private:
    const UserInterfaceData& config;
    SDL2pp::SDL sdl;
    SDL2pp::Window window;
    SDL2pp::Renderer renderer;
    ConnectionHandler& connection;
    SpriteCreator sprite_creator;

    std::string player_name;
    World world;
    Camera camera;

    UserInterface ui;

    bool keep_running;
    bool just_restored;
    bool is_fullscreen;

    KeyHandler key_handler;

    // Principales
    void pollEvents();

    void render_ui_and_world(int iteration);

    void update_state_from_server();

    void handle_mouse_click(const SDL_Event& event);

    void handle_ui_click(const SDL_Event& event);

    void handle_game_click(const SDL_Event& event);

    bool is_inside_viewport(int x, int y, const SDL2pp::Rect& viewport);

    void handle_mouse_wheel(const SDL_Event& event);

    // Este método aprovecha el funcionamiento del SO para mover la ventana sin bordes
    static SDL_HitTestResult hit_test_callback(SDL_Window*, const SDL_Point* area, void* data);

public:
    ClientGame(ConnectionHandler& connection, std::string& player_name, const std::string& resolution,
               AudioManager& audio_manager, FontManager& font_manager);

    void run();

    Camera initialize_world_and_camera();
};


#endif  // CLIENT_GAME_H
