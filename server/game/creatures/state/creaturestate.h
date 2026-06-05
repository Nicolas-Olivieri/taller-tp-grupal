#ifndef CREATURESTATE_H
#define CREATURESTATE_H

#include <iostream>
#include <memory>

#include "common/direction.h"
#include "server/command/cmd_results/interact/interact_result.h"
#include "server/game/position.h"

class Creature;

class CreatureState {
public:
    virtual InteractResult act(Creature& creature, const Position& position, const Direction& direction) = 0;

    virtual void next(Creature& creature) = 0;

    virtual ~CreatureState() = default;
};

#endif  // CREATURESTATE_H
