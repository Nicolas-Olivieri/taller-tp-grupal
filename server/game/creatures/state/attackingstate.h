#ifndef ATTACKINGSTATE_H
#define ATTACKINGSTATE_H

#include "creaturestate.h"

class AttackingState: public CreatureState {
public:
    InteractResult act(Creature& creature, const Position& position, const Direction& direction) override;

    void next(Creature& creature) override;
};

#endif  // ATTACKINGSTATE_H
