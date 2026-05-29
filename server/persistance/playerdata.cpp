#include "playerdata.h"

// TODO: el jugador podría empezar con ciertas cosas, como oro o un arma
// Constructor para registrar un jugador
PlayerData::PlayerData(uint8_t archetype, uint8_t race, uint8_t body, uint8_t head):
        xp_level(1), has_played_before(0), archetype(archetype), race(race), body(body), head(head) {}

// TODO: en este constructor, actualizamos la data persistida
PlayerData::PlayerData(const Player& player) {
    Position position(player.get_position());
    position_x = position.get_x();
    position_y = position.get_y();
}
