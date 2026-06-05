#ifndef CREATURESTATE_H
#define CREATURESTATE_H

#include <iostream>
#include <memory>

#include "common/direction.h"
#include "server/game/creatures/updatestatus.h"
#include "server/game/position.h"

class Creature;

class CreatureState {
public:
    virtual CreatureUpdateStatus act(Creature& creature, const Position& position,
                                     const Direction& direction) = 0;

    virtual void next(Creature& creature) = 0;

    virtual ~CreatureState() = default;
};

#endif  // CREATURESTATE_H
