#ifndef KILLABLE_H
#define KILLABLE_H

#include "server/game/stats/stats.h"

#include "interactive.h"


class Player;


class Killable: public Interactive {
protected:
    bool is_meditating;
    int attack_cooldown;
    int move_cooldown;

    Stats stats;

    Killable(int attack_cooldown, int move_cooldown, int agility, int constitution, int intelligence,
             int strength, Archetype archetype, Race race);

public:
    void drop();

    void interact(Player&) override;
};


#endif  // KILLABLE_H
