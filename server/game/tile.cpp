#include "tile.h"


Tile::Tile(const bool is_walkable): is_walkable_(is_walkable), biome(UINT8_MAX), occupant_(nullptr) {}

bool Tile::is_walkable() const { return is_walkable_; }


Interactive* Tile::occupant() const { return occupant_; }

std::stack<Loot>& Tile::get_loot() { return loot_; }

void Tile::add_loot(const Loot& drop) { loot_.push(drop); }

void Tile::add_loot(const std::vector<Loot>& drops) {
    for (const auto& drop: drops) loot_.push(drop);
}

void Tile::occupy(Interactive* occupant) { occupant_ = occupant; }

void Tile::set_biome_id(uint8_t biome_id) { biome = biome_id; }

uint8_t Tile::get_biome_id() {
    if (biome == UINT8_MAX)
        throw UnknownBiome();
    return biome;
}
