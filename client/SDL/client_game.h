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

    bool keep_running;
    bool just_restored;
    const SDL2pp::Rect minimize_button = {977, 5, 20, 20};
    const SDL2pp::Rect close_button = {998, 5, 20, 20};
    const SDL2pp::Rect header_bar = {112, 0, 858, 30};

    bool is_chat_active;
    std::string chat_text;
    const SDL2pp::Rect chat_icon = {5, 190, 45, 30};

    // TODO: acá meter las cosas del chat

    // Principales
    void pollEvents();

    void render_ui_and_world();

    void update_state_from_server();

    void handle_key_down(const SDL_Event& event);

    void handle_mouse_click(const SDL_Event& event);

    void handle_ui_click(const SDL_Event& event);

    void handle_game_click(const SDL_Event& event);

    bool is_inside_viewport(int x, int y, const SDL2pp::Rect& viewport);

    void toggle_chat();

    void handle_chat_events(const SDL_Event& event);

    void handle_mouse_wheel(const SDL_Event& event);

    void handle_text_command(const std::string& text);

    void handle_buy_item_command(const std::string& text);

    void handle_sell_item_command(const std::string& text);

    void handle_deposit_gold_command(const std::string& text);

    void handle_deposit_item_command(const std::string& text);

    void handle_withdraw_gold_command(const std::string& text);

    void handle_withdraw_item_command(const std::string& text);

    void handle_clan_foundation(const std::string& text);

    void send_private_message();

    // Este método aprovecha el funcionamiento del SO para mover la ventana sin bordes
    static SDL_HitTestResult hit_test_callback(SDL_Window*, const SDL_Point* area, void* data);

public:
    ClientGame(ConnectionHandler& connection, std::string& player_name);

    void run();

    Camera initialize_world_and_camera();

    void handle_clan_join(const std::string& text);

    std::string get_clan_name(const std::string& prefix, const std::string& text) const;
};


#endif  // CLIENT_GAME_H
