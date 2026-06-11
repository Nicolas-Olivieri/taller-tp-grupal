#include "attackingstate.h"

#include <memory>

#include "server/game/creatures/creature.h"

#include "followingstate.h"

AttackingState& AttackingState::get() {
    static AttackingState instance;
    return instance;
}

CreatureState* AttackingState::next(Creature& creature) {
    if (!creature.can_reach_target()) {
        std::cout << "attacking -> following" << std::endl;
        return &FollowingState::get();
    }

    return this;
}
