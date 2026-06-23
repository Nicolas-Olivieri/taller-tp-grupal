#include "user_interface.h"

#include "client/config/client_config.h"

UserInterface::UserInterface(SDL2pp::Renderer& renderer, SpriteCreator& sprite_creator,
                             std::string& player_name):
        renderer(renderer),
        config(ClientConfig::get().get_ui_data()),
        sprite_creator(sprite_creator),
        player_name(player_name),
        inventory_ui(sprite_creator, player_name),
        chat_ui(sprite_creator, player_name),
        game_border_ui(sprite_creator.create_sprite(UiElement::SCREEN, config.screen)),
        help_ui(sprite_creator, renderer) {}

void UserInterface::render(const std::string& input, bool is_chat_active) {
    game_border_ui.render();
    inventory_ui.render();
    chat_ui.render(input, is_chat_active);
}

void UserInterface::update_player_state(const std::vector<PlayerInfoDTO>& players_information) {
    const auto player = std::ranges::find_if(players_information, [this](const PlayerInfoDTO& player_info) {
        return player_info.name == player_name;
    });

    if (player == players_information.end()) {
        return;
    }

    inventory_ui.update_player_state(*player);
    chat_ui.update_player_state(*player);
}

void UserInterface::update_chat(const std::vector<ActionDTO>& actions) { chat_ui.update_chat(actions); }

void UserInterface::chat_scroll_to_bottom() { chat_ui.chat_scroll_to_bottom(); }

void UserInterface::chat_scroll_up() { chat_ui.chat_scroll_up(); }

void UserInterface::chat_scroll_down() { chat_ui.chat_scroll_down(); }

bool UserInterface::is_over_chat(const int x, const int y) { return chat_ui.is_over_chat(x, y); }

int UserInterface::get_inventory_slot_at(const int x, const int y) const {
    return inventory_ui.get_slot_at(config.inventory_slots, x, y);
}

std::optional<uint8_t> UserInterface::get_item_in_inventory_slot(const int slot_index) const {
    return inventory_ui.get_item_in_slot(inventory_ui.inventory, slot_index);
}

void UserInterface::bind_item(const int slot_index) { inventory_ui.bind_item(slot_index); }

std::optional<uint8_t> UserInterface::get_bound_item_id() const { return inventory_ui.get_bound_item_id(); }

void UserInterface::clear_bound_item() { inventory_ui.clear_bound_item(); }

int UserInterface::get_equipment_slot_at(const int x, const int y) const {
    return inventory_ui.get_slot_at(config.equipment_slots, x, y);
}

std::optional<uint8_t> UserInterface::get_item_in_equipment_slot(const int slot_index) const {
    return inventory_ui.get_item_in_slot(inventory_ui.equipment, slot_index);
}

void UserInterface::toggle_help(const HelpPage& page) { help_ui.choose_page(page); }

void UserInterface::render_help() { help_ui.render(); }
