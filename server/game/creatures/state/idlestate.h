#ifndef IDLESTATE_H
#define IDLESTATE_H

#include "creaturestate.h"

class IdleState: public CreatureState {
public:
    static IdleState& get();

    CreatureState* next(Creature& creature) override;
};

#endif  // IDLESTATE_H
