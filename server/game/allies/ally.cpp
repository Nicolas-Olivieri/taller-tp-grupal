#include "ally.h"

#include <cassert>

#include "server/game/player/player.h"


Ally::Ally(const Position& position): position(position) {}


InteractResult Ally::interact(Player& player) {
    player.bind_ally(this);
    std::cout << "[Ally] El jugador se vinculó con el aliado" << std::endl;
    return InteractResult(true);
}


Position Ally::get_position() const { return position; }
