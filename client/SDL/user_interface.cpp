#include "user_interface.h"

#include <algorithm>
#include <cassert>
#include <format>
#include <sstream>
#include <unordered_map>

#include "client/config/client_config.h"

// TODO: revisar constantes
#define LINE_SPACING 21
#define MAX_CHAT_HISTORY 100

UserInterface::UserInterface(SDL2pp::Renderer& renderer, std::string& player_name, FontManager& font_manager):
        renderer(renderer),
        font_manager(font_manager),
        inventory_ui(renderer, font_manager, player_name),
        ui_texture(renderer, DATA_PATH "/interfaz_principal.bmp"),
        player_name(player_name),
        clan_name("") {}

void UserInterface::render() {
    inventory_ui.render();
    //renderer.Copy(ui_texture, SDL2pp::NullOpt, SDL2pp::NullOpt);
}

void UserInterface::render_text(const std::string& text, const SDL2pp::Rect& box_limit,
                                const FontType& font_type) const {
    if (text == "")
        return;

    SDL2pp::Font& font = font_manager.get_font(font_type);
    SDL2pp::Texture text_texture(renderer, font.RenderUTF8_Solid(text, white));

    const int text_w = std::min(text_texture.GetWidth(), box_limit.w);
    const int text_h = std::min(text_texture.GetHeight(), box_limit.h);

    SDL2pp::Rect centered_box = {box_limit.x + (box_limit.w - text_w) / 2,
                                 box_limit.y + (box_limit.h - text_h) / 2, text_w, text_h};

    renderer.Copy(text_texture, SDL2pp::NullOpt, centered_box);
}

void UserInterface::render_chat_history() {
    if (chat_history.empty())
        return;

    size_t start = first_visible_message;
    size_t end = std::min(start + get_visible_lines(), chat_history.size());

    for (size_t i = start; i < end; ++i) {
        const auto& [text, color] = chat_history[i];

        if (text.empty())
            continue;

        SDL2pp::Font& font = font_manager.get_font(FontType::UI_CHAT);
        SDL2pp::Texture line_texture(renderer, font.RenderUTF8_Solid(text, color));

        int current_y = history_messages.y + ((i - start) * LINE_SPACING);
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

    SDL2pp::Font& font = font_manager.get_font(FontType::UI_CHAT);
    SDL2pp::Texture text_texture(renderer, font.RenderUTF8_Blended(display_text, white));

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
    inventory_ui.update_player_state(players_information);
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
            case ActionType::LIST_BANK:
                handle_list_bank(action);
                break;
            case ActionType::CLAN_MESSAGE:
                handle_clan_message(action);
                break;
            default:
                break;
        }
    }
}

void UserInterface::enqueue_message(const std::string& message, SDL_Color color) {
    const size_t visible_lines = get_visible_lines();
    bool is_at_bottom = first_visible_message + visible_lines >= chat_history.size();

    chat_history.push_back({message, color});

    if (chat_history.size() > MAX_CHAT_HISTORY)
        chat_history.pop_front();

    if (!is_at_bottom)
        return;

    chat_scroll_to_bottom();
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

void UserInterface::handle_list_bank(const ActionDTO& action) {
    const ListBankDTO& bank = action.bank;
    if (bank.receiver != player_name) {
        return;
    }

    const SDL_Color color = assign_message_color(bank.type);

    enqueue_message(std::format("    - Oro - Cantidad: {}", bank.gold), color);
    for (const auto& [item_id, amount]: bank.items) {
        std::string item_name = ClientConfig::get().get_item_name(item_id);
        enqueue_message(std::format("    - {} - Cantidad: {}", item_name, amount), color);
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

void UserInterface::chat_scroll_up() {
    if (first_visible_message > 0)
        --first_visible_message;
}

void UserInterface::chat_scroll_down() {
    size_t max_start;
    if (chat_history.size() > get_visible_lines()) {
        max_start = chat_history.size() - get_visible_lines();
    } else {
        max_start = 0;
    }

    if (first_visible_message < max_start)
        ++first_visible_message;
}

size_t UserInterface::get_visible_lines() const { return history_messages.h / LINE_SPACING; }


void UserInterface::chat_scroll_to_bottom() {
    const size_t visible_lines = get_visible_lines();

    if (chat_history.size() > visible_lines) {
        first_visible_message = chat_history.size() - visible_lines;
    } else {
        first_visible_message = 0;
    }
}

bool UserInterface::is_over_chat(const int x, const int y) {
    SDL2pp::Point click_position(x, y);
    return history_messages.Contains(click_position) || input_box.Contains(click_position);
}

int UserInterface::get_inventory_slot_at(const int x, const int y) const {
    return inventory_ui.get_slot_at(inventory_ui.inventory_slots, x, y);
}

std::optional<uint8_t> UserInterface::get_item_in_inventory_slot(const int slot_index) const {
    return inventory_ui.get_item_in_slot(inventory_ui.inventory, slot_index);
}

void UserInterface::bind_item(const int slot_index) {
    inventory_ui.bind_item(slot_index);
}

std::optional<uint8_t> UserInterface::get_bound_item_id() const {
    return inventory_ui.get_bound_item_id();
}

void UserInterface::clear_bound_item() {
    inventory_ui.clear_bound_item();
}

int UserInterface::get_equipment_slot_at(const int x, const int y) const {
    return inventory_ui.get_slot_at(inventory_ui.equipment_slots, x, y);
}

std::optional<uint8_t> UserInterface::get_item_in_equipment_slot(const int slot_index) const {
    return inventory_ui.get_item_in_slot(inventory_ui.equipment, slot_index);
}

void UserInterface::handle_clan_message(const ActionDTO& action) {
    assert(action.action == ActionType::CLAN_MESSAGE);
    if (clan_name.empty())
        return;

    const ClanMessageDTO dto = action.clan_msg;

    if (clan_name != dto.receiver_clan || player_name == dto.sender)
        return;

    SDL_Color color = assign_message_color(MessageType::CLAN);

    enqueue_message(dto.content, color);
}
