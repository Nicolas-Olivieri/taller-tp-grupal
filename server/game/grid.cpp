#include "grid.h"


Grid::Grid(const int width, const int height, const GridMatrixDTO& grid_data): width_(width), height_(height) {
    for (const auto& row : grid_data.walkable_tiles) {
        std::vector<Tile> tile_row;
        for (const auto& tile_value : row) {
            tile_row.emplace_back(tile_value);
        }
        tiles_.emplace_back(tile_row);
    }
}


Tile& Grid::get_tile(const Position& position) {
    const int x = position.get_x();
    const int y = position.get_y();

    if (x < 0 or x >= width_ or y < 0 or y >= height_)
        throw std::out_of_range("Position out of range");

    return tiles_[y][x];
}


Position Grid::spawn() const {
    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_int_distribution get_random_width(0, width_-1);
    std::uniform_int_distribution get_random_height(0, height_-1);
    int x, y;
    do {
        x = get_random_width(generator);
        y = get_random_height(generator);
    } while (not tiles_[y][x].is_walkable() or tiles_[y][x].occupant() != nullptr);

    return Position(x, y);
}
