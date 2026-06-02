#include "ally.h"

#include "server/game/player/player.h"


Ally::Ally(const Position& position): position(position) {}


bool Ally::interact(Player& player) {
    player.bind_ally(this);
    std::cout << "[Ally] El jugador se vinculó con el sacerdote" << std::endl;
    return false;
}


Position Ally::get_position() const { return position; }
