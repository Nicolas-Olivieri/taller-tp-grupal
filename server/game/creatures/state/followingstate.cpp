#include "followingstate.h"

#include <memory>

#include "server/game/creatures/creature.h"

#include "attackingstate.h"
#include "idlestate.h"

CreatureUpdateStatus FollowingState::act(Creature& creature, const Position& position,
                                         const Direction& direction) {
    if (creature.can_move()) {
        creature.update_position(position, direction);
    }

    return CreatureUpdateStatus();
}

void FollowingState::next(Creature& creature) {
    if (creature.can_reach_target() && creature.can_attack()) {
        creature.set_state(std::make_unique<AttackingState>());
    } else if (!creature.is_targeting_someone() || creature.should_stop_targeting()) {
        creature.stop_targeting();
        creature.set_state(std::make_unique<IdleState>());
    }
}
