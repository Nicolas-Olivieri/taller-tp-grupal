#ifndef IDLESTATE_H
#define IDLESTATE_H

#include "creaturestate.h"

class IdleState: public CreatureState {
public:
    CreatureUpdateStatus act(Creature& creature, const Position& position,
                             const Direction& direction) override;

    void next(Creature& creature) override;
};

#endif  // IDLESTATE_H
