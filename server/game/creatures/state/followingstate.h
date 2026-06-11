#ifndef FOLLOWINGSTATE_H
#define FOLLOWINGSTATE_H

#include "creaturestate.h"

class FollowingState: public CreatureState {
public:
    static FollowingState& get();

    CreatureState* next(Creature& creature) override;
};

#endif  // FOLLOWINGSTATE_H
