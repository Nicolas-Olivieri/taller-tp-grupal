#include "playerdata.h"

#include <algorithm>
#include <cstring>

#include "server/game/player/player.h"

// TODO: el jugador podría empezar con ciertas cosas, como oro o un arma
// Constructor para registrar un jugador
PlayerData::PlayerData(uint8_t archetype, uint8_t race, uint8_t body, uint8_t head):
        xp_level(1), has_played_before(0), archetype(archetype), race(race), body(body), head(head) {}

// TODO: en este constructor, actualizamos la data persistida
PlayerData::PlayerData(const Player& player) {
    const Stats& stats(player.get_stats());
    const Position& position(player.get_position());
    const Equipment& equipment = player.get_equipment();

    current_xp_amount = stats.experience.get_current_amount();
    current_hp = stats.health.get_current();
    current_mana = stats.mana.get_current();

    current_gold = player.get_safe_gold() + player.get_excess_gold();
    bank_gold = player.get_bank_gold();

    position_x = position.get_x();
    position_y = position.get_y();

    xp_level = stats.experience.get_level();

    weapon_id = equipment.weapon;
    helmet_id = equipment.helmet;
    armor_id = equipment.armor;

    std::memset(inventory, 0, INVENTORY_AMOUNT);
    std::memset(inventory_amounts, 0, INVENTORY_AMOUNT);

    size_t i = 0;
    for (const auto& [item_id, amount]: player.get_inventory_items()) {
        if (i >= INVENTORY_AMOUNT)
            break;

        inventory[i] = item_id;
        inventory_amounts[i] = amount;
        i++;
    }

    std::memset(bank, 0, BANK_AMOUNT);
    std::memset(bank_amounts, 0, BANK_AMOUNT);

    i = 0;
    for (const auto& [item_id, amount]: player.get_bank_items()) {
        if (i >= BANK_AMOUNT) {
            break;
        }

        bank[i] = item_id;
        bank_amounts[i] = amount;
        i++;
    }

    has_played_before = 1;
    archetype = stats.archetype_id;
    race = stats.race_id;
    body = player.get_body();
    head = player.get_head();

    is_founder = player.is_clan_founder();

    std::memcpy(clan, player.get_clan_name().data(),
                std::min(static_cast<int>(player.get_clan_name().size()), CLAN_NAME));
}
