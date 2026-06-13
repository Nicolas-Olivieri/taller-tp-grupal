#ifndef ATTACKINGSTATE_H
#define ATTACKINGSTATE_H

#include "creaturestate.h"

class AttackingState: public CreatureState {
public:
    static AttackingState& get();

    CreatureState* next(Creature& creature) override;
};

#endif  // ATTACKINGSTATE_H
