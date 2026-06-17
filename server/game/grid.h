#ifndef GRID_H
#define GRID_H

#include <random>
#include <vector>

#include "common/direction.h"
#include "common/dto/snapshot/map/grid_matrix.h"

#include "position.h"
#include "tile.h"


// forward declaration
class Position;


class Grid {
private:
    int width_;
    int height_;
    std::vector<std::vector<Tile>> tiles_;
    std::vector<Direction> directions;

public:
    Grid();

    Grid(int width, int height, const GridMatrixDTO& grid_data);

    Tile& get_tile(const Position& position);

    bool is_tile_available(int x, int y) const;

    Position spawn() const;

    Position spawn_near(const std::vector<Position>& positions) const;

    Direction closest_movement(const Position& position, const Position& target) const;

    Direction random_movement(const Position& current) const;

private:
    void add_near_positions(std::vector<Position>& near_positions, uint16_t pos_x, uint16_t pos_y) const;
};


#endif  // GRID_H
