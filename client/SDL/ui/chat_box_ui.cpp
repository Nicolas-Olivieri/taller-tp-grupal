#include "chat_box_ui.h"

#include <algorithm>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "client/config/client_config.h"

ChatBoxUI::ChatBoxUI(SpriteCreator& sprite_creator, const std::string& username):
        creator(sprite_creator),
        ui_config(ClientConfig::get().get_ui_data()),
        chat_config(ClientConfig::get().get_chat_data()),
        ui(creator.create_sprite(UiElement::CHAT, ui_config.chat_box)),
        player_name(username),
        white(ClientConfig::get().get_color_data().white),
        input_msg(creator.create_sprite(ui_config.input_box, "", FontType::UI_CHAT, white)) {
    init_texts();
    init_color_msg();
}

void ChatBoxUI::init_texts() {
    const size_t start = first_visible_message;
    const size_t end = start + get_visible_lines();
    for (size_t i = start; i < end; ++i) {
        const SDL2pp::Rect history_messages = ui_config.history_messages;
        const int current_y = history_messages.y + ((i - start) * ui_config.chat_line_spacing);
        const SDL2pp::Rect box(history_messages.x, current_y, history_messages.w, history_messages.h);

        TextSprite msg_sprite = creator.create_sprite(box, "", FontType::UI_CHAT, white);
        visible_texts.push_back(std::move(msg_sprite));
    }
}

void ChatBoxUI::init_color_msg() {
    assert(msg_type_to_color.empty());
    const ColorData& config = ClientConfig::get().get_color_data();

    msg_type_to_color.insert({{MessageType::SYSTEM, config.yellow},
                              {MessageType::PRIVATE, config.grey},
                              {MessageType::GLOBAL, config.white},
                              {MessageType::CLAN, config.green},
                              {MessageType::ERROR, config.red},
                              {MessageType::ALLY, config.light_blue}});
}

void ChatBoxUI::render(const std::string& input, bool is_chat_active) {
    ui.render();
    render_chat_history();
    render_chat_input(input, is_chat_active);
}

void ChatBoxUI::render_chat_history() {
    if (chat_history.empty())
        return;

    const size_t start = first_visible_message;
    const size_t end = std::min(start + get_visible_lines(), chat_history.size());

    for (size_t i = start; i < end; ++i) {
        auto& [text, color] = chat_history[i];
        auto& msg_sprite = visible_texts[i - start];

        if (text.empty())
            continue;

        msg_sprite.set_text(text);
        msg_sprite.set_color(color);

        msg_sprite.render_left();
    }
}

void ChatBoxUI::render_chat_input(const std::string& input, bool is_chat_active) {
    std::string display_text(input);
    if (is_chat_active)
        add_twinkling_bar(display_text);

    if (display_text.empty())
        return;
    assert(input_msg.get_color() == white);

    input_msg.set_text(display_text);
    input_msg.render_left();
}

void ChatBoxUI::add_twinkling_bar(std::string& display_text) {
    // TODO: cada 500 ms de SDL lo agrega, reemplazar constantes
    if ((SDL_GetTicks() / 500) % 2 == 0)
        display_text += "|";
}

void ChatBoxUI::update_chat(const std::vector<ActionDTO>& actions) {
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

void ChatBoxUI::enqueue_message(const std::string& message, SDL2pp::Color color) {
    const size_t visible_lines = get_visible_lines();
    const bool is_at_bottom = first_visible_message + visible_lines >= chat_history.size();

    chat_history.push_back(MsgData{message, color});

    if (chat_history.size() > chat_config.max_chat_history)
        chat_history.pop_front();

    if (!is_at_bottom)
        return;

    chat_scroll_to_bottom();
}

void ChatBoxUI::handle_chat_message(const ActionDTO& action) {
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

void ChatBoxUI::handle_chat_list(const ActionDTO& action) {
    const ChatListDTO& list = action.list;
    if (list.receiver != player_name)
        return;

    SDL_Color color = assign_message_color(list.type);
    for (const std::string& line: list.lines) {
        enqueue_message(line, color);
    }
}

void ChatBoxUI::handle_list_bank(const ActionDTO& action) {
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

void ChatBoxUI::handle_list_items(const ActionDTO& action) {
    const ListItemsDTO& list = action.items;
    if (list.receiver != player_name) {
        return;
    }

    SDL_Color color = assign_message_color(list.type);
    for (const auto& [item_id, price]: list.items) {
        std::string item_name = ClientConfig::get().get_item_name(item_id);
        enqueue_message(std::format("    - {} - Precio: {} monedas de oro", item_name, price), color);
    }
}

void ChatBoxUI::handle_clan_message(const ActionDTO& action) {
    assert(action.action == ActionType::CLAN_MESSAGE);
    if (clan_name.empty())
        return;

    const ClanMessageDTO dto = action.clan_msg;

    if (clan_name != dto.receiver_clan || player_name == dto.sender)
        return;

    SDL_Color color = assign_message_color(MessageType::CLAN);

    enqueue_message(dto.content, color);
}


SDL2pp::Color ChatBoxUI::assign_message_color(const MessageType& type) const {
    assert(msg_type_to_color.contains(type));

    return msg_type_to_color.at(type);
}

void ChatBoxUI::chat_scroll_up() {
    if (first_visible_message > 0)
        --first_visible_message;
}

void ChatBoxUI::chat_scroll_down() {
    size_t max_start;
    if (chat_history.size() > get_visible_lines()) {
        max_start = chat_history.size() - get_visible_lines();
    } else {
        max_start = 0;
    }

    if (first_visible_message < max_start)
        ++first_visible_message;
}

void ChatBoxUI::chat_scroll_to_bottom() {
    const size_t visible_lines = get_visible_lines();

    if (chat_history.size() > visible_lines) {
        first_visible_message = chat_history.size() - visible_lines;
    } else {
        first_visible_message = 0;
    }
}

bool ChatBoxUI::is_over_chat(const int x, const int y) {
    SDL2pp::Point click_position(x, y);
    return ui_config.history_messages.Contains(click_position) ||
           ui_config.input_box.Contains(click_position);
}

size_t ChatBoxUI::get_visible_lines() const {
    return ui_config.history_messages.h / ui_config.chat_line_spacing;
}

void ChatBoxUI::update_player_state(const PlayerInfoDTO& player) {
    assert(player.name == player_name);

    clan_name = player.clan.name;
}
