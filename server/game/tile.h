#ifndef TILE_H
#define TILE_H

#include <stack>
#include <vector>

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

    bool has_loot() const;

    Loot take_loot();

    const Loot& peek_loot() const;

    void add_loot(uint8_t item);

    void add_loot(uint16_t gold);

    void add_loot(const std::vector<Loot>& drops);

    void occupy(Interactive* occupant);
};


#endif  // TILE_H
