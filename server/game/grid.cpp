#include "grid.h"


Grid::Grid(const int width, const int height): width_(width), height_(height) {
    for (int x = 0; x < width; ++x) {
        std::vector<Tile> row;
        for (int y = 0; y < height; ++y) {
            if (y < height_ / 2) {
                row.emplace_back(true);
            } else {
                row.emplace_back(false);
            }
        }
        tiles_.emplace_back(row);
    }
}


Tile& Grid::get_tile(const Position& position) {
    const int x = position.get_x();
    const int y = position.get_y();

    if (x < 0 or x >= width_ or y < 0 or y >= height_) {
        throw std::out_of_range("Position out of range");
    }

    return tiles_[x][y];
}


Position Grid::spawn() const {
    /* TODO: solución temporal, revisar cómo manejar el spawn inicial de jugadores
    
    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_int_distribution get_random_width(0, width_);
    std::uniform_int_distribution get_random_height(0, height_);
    int x, y;
    do {
        x = get_random_width(generator);
        y = get_random_height(generator);
    } while (not tiles_[x][y].is_walkable() or
             tiles_[x][y].occupant() != nullptr); */
    int x = 0, y = 0;
    return Position(x, y);
}
