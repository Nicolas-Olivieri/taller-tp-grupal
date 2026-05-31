#ifndef PLAYERDATA_H
#define PLAYERDATA_H

#include <cstddef>
#include <cstdint>

// TODO: constantes temporales, sincronizar dependencias
#define INVENTORY_AMOUNT 9
#define BANK_AMOUNT 16
#define CLAN_NAME 15

// Forward Declaration para evitar dependencia circular
class Player;

// TODO: Considerar cambiar algunos uint por enums

#pragma pack(push, 1)

struct PlayerData {
    uint32_t current_xp_amount{0};
    uint16_t current_hp{0};
    uint16_t current_mana{0};
    uint16_t current_gold{0};  // tanto "seguro" como "exceso"
    uint16_t bank_gold{0};
    uint16_t position_x{0};
    uint16_t position_y{0};
    uint8_t xp_level{0};
    uint8_t weapon_id{0};
    uint8_t helmet_id{0};
    uint8_t armor_id{0};
    uint8_t shield_id{0};
    uint8_t inventory[INVENTORY_AMOUNT]{0};
    uint8_t inventory_amounts[INVENTORY_AMOUNT]{0};
    uint8_t bank[BANK_AMOUNT]{0};
    uint8_t bank_amounts[BANK_AMOUNT]{0};
    uint8_t clan[CLAN_NAME]{0};
    uint8_t is_founder{0};         // Nunca cambia solo cuando vale 1
    uint8_t has_played_before{0};  // Nunca cambia solo cuando vale 1

    // Estos campos son constantes para todo jugador. Los variables agregarlos encima de estos
    uint8_t archetype{0};
    uint8_t race{0};
    uint8_t body{0};
    uint8_t head{0};

    PlayerData() = default;

    PlayerData(uint8_t archetype, uint8_t race, uint8_t body, uint8_t head);

    explicit PlayerData(const Player& player);
};

#pragma pack(pop)

#endif  // PLAYERDATA_H
