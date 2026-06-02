#include "user_interface.h"

#include <cassert>

// TODO: revisar constantes
#define FONT_SIZE 19
#define MAX_HISTORY_MSGS 4

UserInterface::UserInterface(SDL2pp::Renderer& renderer, std::string& player_name):
        renderer(renderer),
        font(DATA_PATH "/augusta.ttf", FONT_SIZE),
        ui_texture(renderer, DATA_PATH "/interfaz_principal.bmp"),
        player_name(player_name) {}

void UserInterface::render() { renderer.Copy(ui_texture, SDL2pp::NullOpt, SDL2pp::NullOpt); }

void UserInterface::render_fields() { assert(false); }

void UserInterface::render_chat_history() {
    if (chat_history.empty())
        return;

    // TODO: constantes
    int start_x = 20;
    int start_y = 33;
    int line_spacing = 21;
    int max_width = 600;

    for (size_t i = 0; i < chat_history.size(); ++i) {
        if (chat_history[i].empty())
            continue;

        SDL2pp::Texture line_texture(renderer, font.RenderText_Solid(chat_history[i], text_color));

        int current_y = start_y + (i * line_spacing);
        int text_w = line_texture.GetWidth();
        int text_h = line_texture.GetHeight();

        cut_text_if_necessary(text_w, max_width);

        renderer.Copy(line_texture, SDL2pp::Rect(0, 0, text_w, text_h),
                      SDL2pp::Rect(start_x, current_y, text_w, text_h));
    }
}

void UserInterface::render_chat_input(const std::string& input, bool is_chat_active) {
    std::string display_text(input);

    if (is_chat_active)
        add_twinkling_bar(display_text);

    if (display_text.empty())
        return;

    SDL2pp::Texture text_texture(renderer, font.RenderText_Blended(display_text, text_color));

    int text_w = text_texture.GetWidth();
    int text_h = text_texture.GetHeight();

    // TODO: constantes
    int max_width = 580;
    int start_x = 40;
    int start_y = 120;

    cut_text_if_necessary(text_w, max_width);

    renderer.Copy(text_texture, SDL2pp::Rect(0, 0, text_w, text_h),
                  SDL2pp::Rect(start_x, start_y, text_w, text_h));
}

void UserInterface::cut_text_if_necessary(int& text_width, int max_width) {
    // TODO: esto corta la línea si se pasa del ancho
    if (text_width > max_width)
        text_width = max_width;
}

void UserInterface::add_twinkling_bar(std::string& display_text) {
    // TODO: cada 500 ms de SDL lo agrega, reemplazar constantes
    if ((SDL_GetTicks() / 500) % 2 == 0)
        display_text += "|";
}

void UserInterface::update_player_state(const std::vector<PlayerInfoDTO>& /* players_information */) {
    assert(false);
}

void UserInterface::update_chat(const std::vector<ActionDTO>& actions) {
    for (const auto& action: actions) {
        if (action.action != ActionType::MESSAGE)
            continue;

        ChatMessageDTO msg = action.chat_message;

        if (msg.visibility == MessageVisibility::PRIVATE &&
            (player_name == msg.receiver || player_name == msg.sender)) {
            std::string formatted_msg = "<" + msg.sender + "> " + msg.content;
            enqueue_message(formatted_msg);
        }
    }
}

void UserInterface::enqueue_message(const std::string& message) {
    chat_history.push_back(message);
    if (chat_history.size() > MAX_HISTORY_MSGS)
        chat_history.pop_front();
}
