#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H


#include <deque>
#include <string>
#include <vector>

#include <SDL2pp/Font.hh>

#include "SDL2pp/Renderer.hh"
#include "SDL2pp/Texture.hh"
#include "common/dto/snapshot/actions/action.h"
#include "common/dto/snapshot/info/playerinfo.h"

class UserInterface {
private:
    SDL2pp::Renderer& renderer;
    SDL2pp::Font font;
    // TODO convertirlo en un sprite
    SDL2pp::Texture ui_texture;
    std::string& player_name;

    // Blanco
    SDL_Color text_color = {255, 255, 255, 255};

    std::deque<std::string> chat_history;

    void enqueue_message(const std::string& message);

    void cut_text_if_necessary(int& text_width, int max_width);

    void add_twinkling_bar(std::string& display_text);

public:
    UserInterface(SDL2pp::Renderer& renderer, std::string& player_name);

    void render();

    // TODO capaz conviene que el render lo haga todo solo, todavía no lo sé
    void render_fields();

    void render_chat_history();

    void render_chat_input(const std::string& input, bool is_chat_active);

    void update_player_state(const std::vector<PlayerInfoDTO>& players_information);

    void update_chat(const std::vector<ActionDTO>& actions);
};


#endif  // USER_INTERFACE_H
