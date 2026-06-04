#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H


#include <deque>
#include <string>
#include <utility>
#include <vector>

#include <SDL2pp/Font.hh>

#include "SDL2pp/Renderer.hh"
#include "SDL2pp/Texture.hh"
#include "common/dto/snapshot/actions/action.h"
#include "common/dto/snapshot/info/playerinfo.h"

struct BarValue {
    SDL2pp::Texture& texture;
    size_t current;
    size_t max;
};

class UserInterface {
private:
    SDL2pp::Renderer& renderer;
    SDL2pp::Font user_font;
    /* SDL2pp::Font clan_font; */
    SDL2pp::Font menu_title_font;
    SDL2pp::Font menu_font;
    SDL2pp::Font chat_font;
    // TODO convertirlo en un sprite
    SDL2pp::Texture ui_texture;
    std::string& player_name;

    std::deque<std::string> chat_history;

    /* std::vector<std::pair<SDL2pp::Rect, std::string>> fixed_values; */
    std::vector<std::pair<SDL2pp::Rect, BarValue>> recoverable_values;

    // Blanco
    SDL_Color text_color = {255, 255, 255, 255};
    // Verde
    SDL_Color health_color = {62, 174, 57, 255};
    // Azul
    SDL_Color mana_color = {13, 134, 213, 255};

    SDL2pp::Rect history_messages = {20, 35, 600, 147};
    SDL2pp::Rect input_box = {45, 190, 580, 25};

    SDL2pp::Rect username_rect = {770, 55, 240, 35};
    /* SDL2pp::Rect clan_name = {770, 100, 240, 20}; */
    SDL2pp::Rect inventory_rect = {770, 157, 240, 35};
    SDL2pp::Rect stats_rect = {770, 519, 240, 35};

    SDL2pp::Rect health_rect = {791, 599, 216, 15};
    SDL2pp::Texture health_texture;
    SDL2pp::Rect mana_rect = {791, 627, 216, 15};
    SDL2pp::Texture mana_texture;
    SDL2pp::Rect xp_rect = {838, 658, 171, 15};
    SDL2pp::Texture xp_texture;

    void enqueue_message(const std::string& message);

    void cut_text_if_necessary(int& text_width, int max_width);

    void add_twinkling_bar(std::string& display_text);

    void render_text(const std::string& text, const SDL2pp::Rect& box_limit, SDL2pp::Font& font);

    void render_recoverable_value(const SDL2pp::Rect& box, const BarValue& value);

    void handle_chat_message(const ActionDTO& action);

    void handle_chat_list(const ActionDTO& action);

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
