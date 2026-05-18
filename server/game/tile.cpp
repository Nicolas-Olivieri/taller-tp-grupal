#include "tile.h"


Tile::Tile(const bool is_walkable):
        is_walkable_(is_walkable), occupant_(nullptr) {}


Tile::Tile(Interactive* occupant):
        is_walkable_(true), occupant_(occupant), loot_(std::stack<Loot>()) {}


bool Tile::is_walkable() const { return is_walkable_; }


Interactive* Tile::occupant() const { return occupant_; }


std::stack<Loot>& Tile::get_loot() { return loot_; }


void Tile::occupy(Interactive* occupant) { occupant_ = occupant; }


Loot Tile::take_loot() {
    const auto loot = loot_.top();
    loot_.pop();
    return loot;
}
