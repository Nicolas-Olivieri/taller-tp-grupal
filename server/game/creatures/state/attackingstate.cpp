#include "attackingstate.h"

#include <memory>

#include "server/game/creatures/creature.h"

#include "followingstate.h"

CreatureUpdateStatus AttackingState::act(Creature& creature, const Position& position,
                                         const Direction& direction) {
    if (creature.can_move()) {
        std::cout << "creature se acerca" << std::endl;
        creature.update_position(position, direction);
    }

    if (creature.can_attack() && creature.can_reach_target()) {
        std::cout << "creature atacó" << std::endl;
        return creature.attack_player();
    }

    return CreatureUpdateStatus();
}

void AttackingState::next(Creature& creature) {
    if (!creature.can_reach_target() || !creature.can_attack()) {
        creature.set_state(std::make_unique<FollowingState>());
        std::cout << "creature te está siguiendo" << std::endl;
    }
}
