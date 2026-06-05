#include "ally.h"

#include "server/game/player/player.h"


Ally::Ally(const Position& position, const AllyType& type): position(position), type(type) {}


InteractResult Ally::interact(Player& player) {
    player.bind_ally(this);
    std::cout << "[Ally] El jugador se vinculó con el aliado" << std::endl;

    return InteractResult(type);
}


Position Ally::get_position() const { return position; }


AllyType Ally::get_type() const { return type; }
