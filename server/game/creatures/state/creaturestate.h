#ifndef CREATURESTATE_H
#define CREATURESTATE_H

#include <iostream>
#include <memory>

#include "common/direction.h"
#include "server/game/creatures/updatestatus.h"
#include "server/game/position.h"

class Creature;

/*
Las implementaciones de CreatureState van a seguir el patrón Flyweight:
Aprovechando que no deberían tener un estado, solo existiría una única instancia de ellas en memoria, la cual
todas las creatures usan.
*/
class CreatureState {
public:
    virtual CreatureState* next(Creature& creature) = 0;

    virtual ~CreatureState() = default;
};

#endif  // CREATURESTATE_H
