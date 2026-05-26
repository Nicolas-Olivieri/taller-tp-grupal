#include "playerdata.h"

// TODO: el jugador podría empezar con ciertas cosas, como oro o un arma
// Constructor para registrar un jugador
PlayerData::PlayerData(uint8_t archetype, uint8_t race, uint8_t body, uint8_t head):
        archetype(archetype),
        race(race),
        body(body),
        head(head),
        has_played_before(false),
        xp_level(1),
        current_xp_amount(0),
        current_gold(0),
        bank_gold(0),
        inventory({0}),
        inventory_amounts({0}),
        bank({0}),
        bank_amounts({0}),
        weapon_id(0),
        helmet_id(0),
        armor_id(0),
        shield_id(0),
        clan({0}),
        is_founder(0),
        // los próximos 4 son valores placeholder, no deberían usarse los valores guardados para cargar los
        // datos del jugador en su primer instancia
        current_hp(0),
        current_mana(0),
        position_x(0),
        position_y(0) {}

PlayerData::PlayerData(const Player& player) {
    Position position(player.get_position());
    position_x = position.get_x();
    position_y = position.get_y();
}
