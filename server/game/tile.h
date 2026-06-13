#ifndef TILE_H
#define TILE_H

#include <stack>
#include <vector>

#include "interactive.h"
#include "loot.h"


class Tile {
private:
    bool is_walkable_;
    uint8_t biome;
    Interactive* occupant_;
    std::stack<Loot> loot_;

public:
    explicit Tile(bool is_walkable, uint8_t biome);

    explicit Tile(Interactive* occupant);

    bool is_walkable() const;

    Interactive* occupant() const;

    std::stack<Loot>& get_loot();

    void add_loot(const Loot& drop);

    void add_loot(const std::vector<Loot>& drops);

    void occupy(Interactive* occupant);
};


#endif  // TILE_H
