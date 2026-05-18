#ifndef KILLABLE_H
#define KILLABLE_H

#include "interactive.h"


class Player;


class Killable: public Interactive {
public:
    void drop();

    void interact(Player&) override;
};


#endif  // KILLABLE_H
