#include "inventory_ui.h"

#include <ranges>
#include <vector>

#include <SDL2pp/Rect.hh>

#include "client/config/client_config.h"

InventoryUI::InventoryUI(SpriteCreator& sprite_creator, const std::string& username):
        creator(sprite_creator),
        config(ClientConfig::get().get_ui_data()),
        ui(creator.create_sprite(UiElement::INVENTORY, config.inventory.GetTopLeft())),
        founder_crown(std::nullopt),
        player_name(creator.create_sprite(username_rect, username, FontType::UI_USERNAME, white)),
        clan_name(creator.create_sprite(clan_rect, "", FontType::UI_CLAN, white)),
        inventory_label(creator.create_sprite(inventory_rect, "Inventario", FontType::UI_MENU_TITLE, white)),
        statistics_label(creator.create_sprite(stats_rect, "Estadísticas", FontType::UI_MENU_TITLE, white)),
        xp_level(creator.create_sprite(xp_level_rect, "", FontType::UI_MENU, white)),
        safe_gold(creator.create_sprite(safe_gold_rect, "", FontType::UI_MENU, white)),
        excess_gold(creator.create_sprite(excess_gold_rect, "", FontType::UI_MENU, white)) {
    init_elements();
}

void InventoryUI::init_elements() {
    ProgressBarSprite health = creator.create_sprite(UiElement::HEALTH_BAR, config.health.GetTopLeft(), 0, 1);
    bars.push_back(std::move(health));

    ProgressBarSprite mana = creator.create_sprite(UiElement::MANA_BAR, config.mana.GetTopLeft(), 0, 1);
    bars.push_back(std::move(mana));

    ProgressBarSprite xp = creator.create_sprite(UiElement::XP_BAR, config.xp.GetTopLeft(), 0, 1);
    bars.push_back(std::move(xp));

    for (const auto& position: inventory_slots) {
        HudSprite item = creator.create_sprite(NO_ITEM, position.GetTopLeft(), true);
        inventory.push_back(std::move(item));
    }

    for (const auto& position: equipment_slots) {
        HudSprite item = creator.create_sprite(NO_ITEM, position.GetTopLeft(), false);
        equipment.push_back(std::move(item));
    }

    for (const auto& position: config.equipment_state_slots) {
        TextSprite state = creator.create_sprite(position, "", FontType::UI_MENU, white);
        equipment_state.push_back(std::move(state));
    }
}

void InventoryUI::update_player_state(const std::vector<PlayerInfoDTO>& players_information) {
    const auto player = std::ranges::find_if(players_information, [this](const PlayerInfoDTO& player_info) {
        return player_info.name == player_name.get_text();
    });

    if (player == players_information.end()) {
        return;
    }

    const PlayerStatsDTO& stats(player->stats);
    const EquipmentInfoDTO& equipment_info(player->equipment);

    bars[0].update_values(stats.current_health, stats.max_health);
    bars[1].update_values(stats.current_mana, stats.max_mana);
    bars[2].update_values(stats.current_xp_amount, stats.max_xp_amount);

    xp_level.set_text(std::to_string(stats.xp_level));
    safe_gold.set_text(std::to_string(player->safe_gold));
    excess_gold.set_text(std::to_string(player->excess_gold));

    clan_name.set_text(player->clan.name);
    if (!founder_crown.has_value() && player->clan.is_founder) {
        founder_crown.emplace(creator.create_sprite(UiElement::FOUNDER_CROWN, config.founder.GetTopLeft()));
    }

    int slot = 0;
    for (const auto& [item_id, amount]: player->inventory.items) {
        creator.update_appearance(inventory[slot], item_id, amount);
        slot++;
    }
    for (size_t i = slot; i < inventory_slots.size(); ++i) {
        creator.update_appearance(inventory[i], NO_ITEM, 0);
    }

    const std::vector equipment_data(
            {equipment_info.weapon, equipment_info.shield, equipment_info.helmet, equipment_info.armor});
    for (size_t i = 0; i < equipment.size(); ++i) {
        creator.update_appearance(equipment[i], equipment_data[i].item_id);
        if (i == 0) {
            equipment_state[i].set_text(std::to_string(equipment_data[i].effect));
        } else {
            equipment_state[i].set_text(
                    std::to_string(equipment_data[i].item_id == NO_ITEM ? 0 : equipment_data[i].effect));
        }
    }
}

void InventoryUI::render() {
    ui.render();
    player_name.render();
    clan_name.render();

    if (founder_crown.has_value()) {
        founder_crown->render();
    }

    inventory_label.render();
    for (auto& item: inventory) {
        item.render();
    }
    for (auto& item: equipment) {
        item.render();
    }

    safe_gold.render();
    excess_gold.render();
    statistics_label.render();
    for (auto& bar: bars) {
        bar.render();
    }
    xp_level.render();
}


// DETECTAR CLICK EN SLOT Y OBTENER ELEMENTO ::::::::::
int InventoryUI::get_slot_at(const std::vector<SDL2pp::Rect>& slots, const int x, const int y) const {
    const SDL2pp::Point click_pos(x, y);
    for (size_t i = 0; i < slots.size(); ++i) {
        if (slots[i].Contains(click_pos))
            return static_cast<int>(i);
    }

    return -1;
}

std::optional<uint8_t> InventoryUI::get_item_in_slot(const std::vector<HudSprite>& slots,
                                                     const int slot_index) const {
    if (slot_index >= 0 and static_cast<size_t>(slot_index) < inventory.size()) {
        uint8_t id = slots[slot_index].get_id();
        if (id != 0)
            return id;
    }

    return std::nullopt;
}


// BINDEAR ITEMS :::::::::::
void InventoryUI::bind_item(const int slot_index) {
    if (bounded_slot.has_value() and bounded_slot.value() == slot_index) {
        clear_bound_item();
        return;
    }

    auto item_id = get_item_in_slot(inventory, slot_index);
    if (item_id.has_value()) {
        if (bounded_slot.has_value()) {
            inventory[bounded_slot.value()].unbind();
        }
        bounded_slot = slot_index;
        inventory[bounded_slot.value()].bind();
    }
}

void InventoryUI::clear_bound_item() {
    inventory[bounded_slot.value()].unbind();
    bounded_slot = std::nullopt;
}

std::optional<uint8_t> InventoryUI::get_bound_item_id() const {
    if (!bounded_slot) {
        return std::nullopt;
    }
    return inventory[bounded_slot.value()].get_id();
}
