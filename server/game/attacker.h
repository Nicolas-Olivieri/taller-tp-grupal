#ifndef ATTACKER_H
#define ATTACKER_H

#include "position.h"

class Attacker {
public:
    virtual int attack() = 0;

    virtual bool can_attack() const = 0;

    virtual bool can_reach(const Position& other_position) const = 0;

    virtual ~Attacker() = default;
};

#endif  // ATTACKER_H
