#ifndef KILLABLE_H
#define KILLABLE_H

#include "interactive.h"


class Player;


class Killable: public Interactive {
protected:
    int attack_cooldown;
    int move_cooldown;

    Killable(int attack_cooldown, int move_cooldown);

public:
    void drop();

    void interact(Player&) override;
};


#endif  // KILLABLE_H
