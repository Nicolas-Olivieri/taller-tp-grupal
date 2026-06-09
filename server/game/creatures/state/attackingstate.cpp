#include "attackingstate.h"

#include <memory>

#include "server/game/creatures/creature.h"

#include "followingstate.h"

CreatureUpdateStatus AttackingState::act(Creature& creature, const Position& position,
                                         const Direction& direction) {
    if (creature.can_move()) {
        creature.update_position(position, direction);
    }

    if (creature.can_attack() && creature.can_reach_target()) {
        return creature.attack_player();
    }

    return CreatureUpdateStatus();
}

void AttackingState::next(Creature& creature) {
    if (!creature.can_reach_target() || !creature.can_attack()) {
        creature.set_state(std::make_unique<FollowingState>());
    }
}
