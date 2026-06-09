#ifndef TILE_H
#define TILE_H

#include <stack>
#include <stdexcept>
#include <vector>

#include "interactive.h"
#include "loot.h"

struct UnknownBiome: public std::runtime_error {
    UnknownBiome(): std::runtime_error("This tile does not belong to an specific biome.") {}
};

class Tile {
private:
    bool is_walkable_;
    uint8_t biome;
    Interactive* occupant_;
    std::stack<Loot> loot_;

public:
    explicit Tile(bool is_walkable);

    Tile(bool is_walkable, uint8_t biome_id);

    bool is_walkable() const;

    Interactive* occupant() const;

    std::stack<Loot>& get_loot();

    void add_loot(const Loot& drop);

    void add_loot(const std::vector<Loot>& drops);

    void occupy(Interactive* occupant);

    void set_biome_id(uint8_t biome_id);

    uint8_t get_biome_id();
};


#endif  // TILE_H
