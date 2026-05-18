#ifndef GRID_H
#define GRID_H

#include <random>
#include <vector>

#include "position.h"
#include "tile.h"


// forward declaration
class Position;


class Grid {
private:
    int width_;
    int height_;
    std::vector<std::vector<Tile>> tiles_;

public:
    explicit Grid(int width, int height);

    Tile& get_tile(const Position& position);

    Position spawn() const;
};


#endif  // GRID_H
