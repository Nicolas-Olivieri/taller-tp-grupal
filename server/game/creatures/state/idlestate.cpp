#include "idlestate.h"

#include <memory>

#include "server/game/creatures/creature.h"

#include "followingstate.h"

CreatureUpdateStatus IdleState::act(Creature& creature, const Position& position,
                                    const Direction& direction) {
    if (creature.can_move()) {
        std::cout << "idle: creature se mueve a x: " << position.get_x() << " y: " << position.get_y()
                  << std::endl;
        creature.update_position(position, direction);
    }

    return CreatureUpdateStatus();
}

void IdleState::next(Creature& creature) {
    if (creature.is_targeting_someone()) {
        creature.set_state(std::make_unique<FollowingState>());
        std::cout << "creature te va a empezar a seguir" << std::endl;
    }
}
