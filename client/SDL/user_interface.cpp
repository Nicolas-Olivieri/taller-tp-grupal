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
#define MENU_FONT_SIZE 17.5
#define CHAT_FONT_SIZE 19
#define ITEM_AMOUNT_FONT_SIZE 14
#define LINE_SPACING 21
#define MAX_CHAT_HISTORY 100

UserInterface::UserInterface(SDL2pp::Renderer& renderer, std::string& player_name):
        renderer(renderer),
        user_font(DATA_PATH FONT, USERNAME_FONT_SIZE),
        /* clan_font(DATA_PATH FONT, FONT_SIZE), */
        menu_title_font(DATA_PATH FONT, MENU_TITLE_FONT_SIZE),
        menu_font(DATA_PATH FONT, MENU_FONT_SIZE),
        chat_font(DATA_PATH FONT, CHAT_FONT_SIZE),
        item_amount_font(DATA_PATH FONT, ITEM_AMOUNT_FONT_SIZE),
        ui_texture(renderer, DATA_PATH "/interfaz_principal.bmp"),
        player_name(player_name),
        current_inventory(),
        current_equipment(),
        health_texture(renderer, DATA_PATH "/barra_vida.bmp"),
        mana_texture(renderer, DATA_PATH "/barra_mana.bmp"),
        xp_texture(renderer, DATA_PATH "/barra_experiencia.bmp") {}

void UserInterface::render() { renderer.Copy(ui_texture, SDL2pp::NullOpt, SDL2pp::NullOpt); }

void UserInterface::render_fields() {
    render_text(player_name, username_rect, user_font);

    render_text("Inventario", inventory_rect, menu_title_font);
    render_inventory();
    render_equipment();

    // TODO: revisar por qué el texto imprime mal las tildes
    render_text("Estadisticas", stats_rect, menu_title_font);

    for (const auto& [box, value]: bar_values) {
        render_bar_value(box, value);
    }

    for (const auto& [box, value]: field_values) {
        render_text(value, box, menu_font);
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

void UserInterface::render_bar_value(const SDL2pp::Rect& box, const BarValue& value) {
    if (value.max == 0)
        return;

    float ratio = static_cast<float>(value.current) / static_cast<float>(value.max);
    int filled_w = static_cast<int>(box.w * ratio);

    renderer.Copy(value.texture, SDL2pp::Rect(0, 0, filled_w, value.texture.GetHeight()),
                  SDL2pp::Rect(box.x, box.y, filled_w, box.h));

    std::stringstream text;
    text << value.current << " / " << value.max;

    render_text(text.str(), box, menu_font);
}

void UserInterface::render_inventory() {
    for (size_t i = 0; i < current_inventory.size(); ++i) {
        const uint8_t item_id = current_inventory[i].item_id;
        const uint8_t amount = current_inventory[i].amount;
        if (item_id != 0) {
            const SDL2pp::Rect& slot = inventory_slots[i];
            render_item(slot, item_id);
            render_item_amount(slot, amount);

            if (bound_slot_index.has_value() and bound_slot_index.value() == static_cast<int>(i)) {
                renderer.SetDrawColor(yellow.r, yellow.g, yellow.b, yellow.a);
                renderer.DrawRect(slot);
                // TODO: Esto ajusta el color por defecto, revisar si los valores son correctos
                renderer.SetDrawColor(0, 0, 0, 255);
            }
        }
    }
}

void UserInterface::render_equipment() {
    for (size_t i = 0; i < current_equipment.size(); ++i) {
        const uint8_t item_id = current_equipment[i];
        if (item_id != 0) {
            render_item(equipment_slots[i], item_id);
        }
    }
}

void UserInterface::render_item(const SDL2pp::Rect& slot, const uint8_t item_id) {
    SDL2pp::Texture& item_tex = get_item_texture(item_id);

    const int texture_w = item_tex.GetWidth();
    const int texture_h = item_tex.GetHeight();

    const int offset_x = slot.x + (slot.w - texture_w) / 2;
    const int offset_y = slot.y + (slot.h - texture_h) / 2;

    SDL2pp::Rect draw_rect(offset_x, offset_y, texture_w, texture_h);

    renderer.Copy(item_tex, SDL2pp::NullOpt, draw_rect);
}

void UserInterface::render_item_amount(const SDL2pp::Rect& slot, const uint8_t amount) {
    if (amount == 0)
        return;

    SDL2pp::Texture text_tex(renderer, item_amount_font.RenderText_Solid(std::to_string(amount), white));

    const int text_w = text_tex.GetWidth();
    const int text_h = text_tex.GetHeight();

    const int text_x = slot.x;
    const int text_y = slot.y - 4;

    renderer.Copy(text_tex, SDL2pp::NullOpt, SDL2pp::Rect(text_x, text_y, text_w, text_h));
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

        SDL2pp::Texture line_texture(renderer, chat_font.RenderText_Solid(text, color));

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

        bar_values.clear();
        field_values.clear();

        // TODO: considerar el resto de estadísticas
        BarValue health_bar = {health_texture, stats.current_health, stats.max_health};
        bar_values.push_back(std::pair(health_rect, health_bar));

        BarValue mana_bar = {mana_texture, stats.current_mana, stats.max_mana};
        bar_values.push_back(std::pair(mana_rect, mana_bar));

        BarValue xp_bar = {xp_texture, stats.current_xp_amount, stats.max_xp_amount};
        bar_values.push_back(std::pair(xp_rect, xp_bar));

        field_values.push_back(std::pair(xp_level_rect, std::to_string(stats.xp_level)));
        field_values.push_back(std::pair(safe_gold_rect, std::to_string(player_info.safe_gold)));
        field_values.push_back(std::pair(excess_gold_rect, std::to_string(player_info.excess_gold)));

        current_inventory.clear();

        for (const auto& [item_id, amount]: player_info.inventory.items) {
            current_inventory.push_back(InventorySlotData(item_id, amount));
        }

        std::ranges::sort(current_inventory,
                          [](const auto& a, const auto& b) { return a.item_id < b.item_id; });

        current_equipment.clear();

        current_equipment = {player_info.equipment.weapon, player_info.equipment.shield,
                             player_info.equipment.helmet, player_info.equipment.armor};

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
            case ActionType::LIST_BANK:
                handle_list_bank(action);
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

SDL2pp::Texture& UserInterface::get_item_texture(const uint8_t item_id) {
    if (not item_textures.contains(item_id)) {
        const std::string icon_path = ClientConfig::get().get_item_icon_path(item_id);
        std::string full_path = DATA_PATH + icon_path;

        item_textures[item_id] = std::make_unique<SDL2pp::Texture>(renderer, full_path);
    }

    return *item_textures[item_id];
}

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
    const SDL2pp::Point click_pos(x, y);
    for (size_t i = 0; i < inventory_slots.size(); ++i) {
        if (inventory_slots[i].Contains(click_pos))
            return static_cast<int>(i);
    }

    return -1;
}

std::optional<uint8_t> UserInterface::get_item_in_slot(const int slot_index) const {
    if (slot_index >= 0 and static_cast<size_t>(slot_index) < current_inventory.size()) {
        uint8_t id = current_inventory[slot_index].item_id;
        if (id != 0)
            return id;
    }

    return std::nullopt;
}

void UserInterface::bind_item(const int slot_index) {
    if (bound_slot_index.has_value() and bound_slot_index.value() == slot_index) {
        clear_bound_item();
        return;
    }

    auto item_id = get_item_in_slot(slot_index);
    if (item_id.has_value()) {
        bound_slot_index = slot_index;
        bound_item_id = item_id.value();
    }
}

std::optional<uint8_t> UserInterface::get_bound_item_id() const { return bound_item_id; }

void UserInterface::clear_bound_item() {
    bound_slot_index = std::nullopt;
    bound_item_id = std::nullopt;
}
