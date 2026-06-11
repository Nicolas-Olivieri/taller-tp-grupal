#include "followingstate.h"

#include <memory>

#include "server/game/creatures/creature.h"

#include "attackingstate.h"
#include "idlestate.h"

FollowingState& FollowingState::get() {
    static FollowingState instance;
    return instance;
}

CreatureState* FollowingState::next(Creature& creature) {
    if (creature.can_reach_target() && creature.can_attack()) {
        std::cout << "following -> attacking" << std::endl;
        return &AttackingState::get();
    } else if (!creature.is_targeting_someone() || creature.should_stop_targeting()) {
        std::cout << "following -> idle" << std::endl;
        creature.stop_targeting();
        return &IdleState::get();
    }

    return this;
}
