#include "tile.h"


Tile::Tile(const bool is_walkable): is_walkable_(is_walkable), occupant_(nullptr) {}


Tile::Tile(Interactive* occupant): is_walkable_(true), occupant_(occupant), loot_(std::stack<Loot>()) {}


bool Tile::is_walkable() const { return is_walkable_; }


Interactive* Tile::occupant() const { return occupant_; }


uint16_t Tile::get_loot_amount() const { return loot_.size(); }

Loot Tile::take_loot() {
    Loot top = loot_.top();
    loot_.pop();

    return top;
}

void Tile::add_loot(uint8_t item) { loot_.push(Loot(item)); }

void Tile::add_loot(uint16_t gold) { loot_.push(Loot(gold)); }

void Tile::add_loot(const std::vector<Loot>& drops) {
    for (const auto& drop: drops) loot_.push(drop);
}

void Tile::occupy(Interactive* occupant) { occupant_ = occupant; }
