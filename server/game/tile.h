#ifndef TILE_H
#define TILE_H

#include <stack>

#include "interactive.h"
#include "loot.h"


class Tile {
private:
    bool is_walkable_;
    Interactive* occupant_;
    std::stack<Loot> loot_;

public:
    explicit Tile(bool is_walkable);

    explicit Tile(Interactive* occupant);

    bool is_walkable() const;

    Interactive* occupant() const;

    std::stack<Loot>& get_loot();

    void occupy(Interactive* occupant);

    Loot take_loot();
};


#endif  // TILE_H
