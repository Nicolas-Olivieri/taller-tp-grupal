#ifndef FOLLOWINGSTATE_H
#define FOLLOWINGSTATE_H

#include "creaturestate.h"

class FollowingState: public CreatureState {
public:
    InteractResult act(Creature& creature, const Position& position, const Direction& direction) override;

    void next(Creature& creature) override;
};

#endif  // FOLLOWINGSTATE_H
