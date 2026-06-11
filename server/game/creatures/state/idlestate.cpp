#include "idlestate.h"

#include <memory>

#include "server/game/creatures/creature.h"

#include "followingstate.h"

IdleState& IdleState::get() {
    static IdleState instance;
    return instance;
}

CreatureState* IdleState::next(Creature& creature) {
    if (creature.is_targeting_someone()) {
        std::cout << "idle -> following" << std::endl;
        return &FollowingState::get();
    }

    return this;
}
