#include "idlestate.h"

#include <memory>

#include "server/game/creatures/creature.h"

#include "followingstate.h"

CreatureUpdateStatus IdleState::act(Creature& creature, const Position& position,
                                    const Direction& direction) {
    if (creature.can_move()) {
        creature.update_position(position, direction);
    }

    return CreatureUpdateStatus();
}

void IdleState::next(Creature& creature) {
    if (creature.is_targeting_someone()) {
        creature.set_state(std::make_unique<FollowingState>());
    }
}
