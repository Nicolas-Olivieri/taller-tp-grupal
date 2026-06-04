#include "user_interface.h"

#include <algorithm>
#include <cassert>
#include <format>
#include <sstream>
#include <unordered_map>

#include "client/config/client_config.h"

// TODO: revisar constantes
#define FONT "/augusta.ttf"
/* #define CLAN_NAME_FONT_SIZE 24 */
#define USERNAME_FONT_SIZE 35
#define MENU_TITLE_FONT_SIZE 20
#define MENU_FONT_SIZE 19
#define CHAT_FONT_SIZE 19
#define LINE_SPACING 21

UserInterface::UserInterface(SDL2pp::Renderer& renderer, std::string& player_name):
        renderer(renderer),
        user_font(DATA_PATH FONT, USERNAME_FONT_SIZE),
        /* clan_font(DATA_PATH FONT, FONT_SIZE), */
        menu_title_font(DATA_PATH FONT, MENU_TITLE_FONT_SIZE),
        menu_font(DATA_PATH FONT, MENU_FONT_SIZE),
        chat_font(DATA_PATH FONT, CHAT_FONT_SIZE),
        ui_texture(renderer, DATA_PATH "/interfaz_principal.bmp"),
        player_name(player_name) {}

void UserInterface::render() { renderer.Copy(ui_texture, SDL2pp::NullOpt, SDL2pp::NullOpt); }

void UserInterface::render_fields() {
    render_text(player_name, username_rect, user_font);
    render_text("Inventario", inventory_rect, menu_title_font);
    // TODO: revisar por qué el texto imprime mal las tildes
    render_text("Estadisticas", stats_rect, menu_title_font);

    for (const auto& [box, value]: recoverable_values) {
        render_recoverable_value(box, value);
    }
}

void UserInterface::render_text(const std::string& text, const SDL2pp::Rect& box_limit, SDL2pp::Font& font) {
    SDL2pp::Texture text_texture(renderer, font.RenderText_Solid(text, white));

    int text_w = std::min(text_texture.GetWidth(), box_limit.w);
    int text_h = std::min(text_texture.GetHeight(), box_limit.h);

    SDL2pp::Rect centered_box = {box_limit.x + (box_limit.w - text_w) / 2,
                                 box_limit.y + (box_limit.h - text_h) / 2, text_w, text_h};

    renderer.Copy(text_texture, SDL2pp::NullOpt, centered_box);
}

void UserInterface::render_recoverable_value(const SDL2pp::Rect& box, const RecoverableValue& value) {
    if (value.max == 0)
        return;

    renderer.SetDrawColor(30, 30, 30, 255);
    renderer.FillRect(box);

    float ratio = static_cast<float>(value.current) / static_cast<float>(value.max);
    int filled_w = static_cast<int>(box.w * ratio);

    SDL2pp::Rect filled_box(box.x, box.y, filled_w, box.h);
    renderer.SetDrawColor(value.color.r, value.color.g, value.color.b, value.color.a);

    renderer.FillRect(filled_box);

    std::stringstream text;
    text << value.current << "/" << value.max;

    render_text(text.str(), box, menu_font);
}

void UserInterface::render_chat_history() {
    if (chat_history.empty())
        return;

    for (size_t i = 0; i < chat_history.size(); ++i) {
        const auto& [text, color] = chat_history[i];

        if (text.empty())
            continue;

        SDL2pp::Texture line_texture(renderer, chat_font.RenderText_Solid(text, color));

        int current_y = history_messages.y + (i * LINE_SPACING);
        int text_w = line_texture.GetWidth();
        int text_h = line_texture.GetHeight();

        cut_text_if_necessary(text_w, history_messages.w);

        renderer.Copy(line_texture, SDL2pp::Rect(0, 0, text_w, text_h),
                      SDL2pp::Rect(history_messages.x, current_y, text_w, text_h));
    }
}

void UserInterface::render_chat_input(const std::string& input, bool is_chat_active) {
    std::string display_text(input);

    if (is_chat_active)
        add_twinkling_bar(display_text);

    if (display_text.empty())
        return;

    SDL2pp::Texture text_texture(renderer, chat_font.RenderText_Blended(display_text, white));

    int text_w = text_texture.GetWidth();
    int text_h = text_texture.GetHeight();

    cut_text_if_necessary(text_w, input_box.w);

    renderer.Copy(text_texture, SDL2pp::Rect(0, 0, text_w, text_h),
                  SDL2pp::Rect(input_box.x, input_box.y, text_w, text_h));
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

void UserInterface::update_player_state(const std::vector<PlayerInfoDTO>& players_information) {
    for (const auto& player_info: players_information) {
        if (player_info.name != player_name)
            continue;

        const PlayerStatsDTO& stats(player_info.stats);

        recoverable_values.clear();

        // TODO: considerar el resto de estadísticas
        RecoverableValue health_bar = {green, stats.current_health, stats.max_health};
        recoverable_values.push_back(std::pair(health_rect, health_bar));

        RecoverableValue mana_bar = {light_blue, stats.current_mana, stats.max_mana};
        recoverable_values.push_back(std::pair(mana_rect, mana_bar));

        break;
    }
}

void UserInterface::update_chat(const std::vector<ActionDTO>& actions) {
    for (const auto& action: actions) {
        switch (action.action) {
            case ActionType::MESSAGE:
                handle_chat_message(action);
                break;
            case ActionType::MESSAGE_LIST:
                handle_chat_list(action);
                break;
            case ActionType::LIST_ITEMS:
                handle_list_items(action);
                break;
            default:
                break;
        }
    }
}

void UserInterface::enqueue_message(const std::string& message, SDL_Color color) {
    chat_history.push_back({message, color});
    if (chat_history.size() > static_cast<size_t>(history_messages.h) / LINE_SPACING)
        chat_history.pop_front();
}

void UserInterface::handle_chat_message(const ActionDTO& action) {
    const ChatMessageDTO& msg = action.chat_message;

    SDL_Color color = assign_message_color(msg.type);

    std::string text = msg.type == MessageType::PRIVATE || msg.type == MessageType::GLOBAL ||
                                       msg.type == MessageType::ALLY ?
                               std::format("[{}] {}", msg.sender, msg.content) :
                               msg.content;

    if ((msg.type == MessageType::PRIVATE && msg.sender == player_name) ||
        (msg.type != MessageType::GLOBAL && msg.receiver == player_name) || msg.type == MessageType::GLOBAL)
        enqueue_message(text, color);
}

void UserInterface::handle_chat_list(const ActionDTO& action) {
    const ChatListDTO& list = action.list;

    SDL_Color color = assign_message_color(list.type);

    if (list.receiver != player_name)
        return;

    for (const std::string& line: list.lines) {
        enqueue_message(line, color);
    }
}

void UserInterface::handle_list_items(const ActionDTO& action) {
    const ListItemsDTO& list = action.items;

    SDL_Color color = assign_message_color(list.type);

    if (list.receiver != player_name) {
        return;
    }

    for (const auto& [item_id, price]: list.items) {
        std::string item_name = ClientConfig::get().get_item_name(item_id);
        enqueue_message(std::format("    - {} - Precio: {} monedas de oro", item_name, price), color);
    }
}

SDL_Color UserInterface::assign_message_color(const MessageType& type) {
    static std::unordered_map<MessageType, SDL_Color> msg_type_to_color = {
            {MessageType::SYSTEM, yellow}, {MessageType::PRIVATE, grey}, {MessageType::GLOBAL, white},
            {MessageType::CLAN, green},    {MessageType::ERROR, red},    {MessageType::ALLY, light_blue}};

    return msg_type_to_color.at(type);
}
