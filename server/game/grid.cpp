#include "grid.h"

#include <algorithm>
#include <utility>

#include "server/config/game_config.h"

Grid::Grid(): width_(0), height_(0) {}

Grid::Grid(const int width, const int height, const GridMatrixDTO& grid_data):
        width_(width),
        height_(height),
        directions({Direction::DOWN, Direction::RIGHT, Direction::LEFT, Direction::UP}) {

    GameConfig& config = GameConfig::get();

    for (uint16_t y = 0; y < height; y++) {
        std::vector<Tile> tile_row;
        tile_row.reserve(width);

        for (uint16_t x = 0; x < width; x++) {
            try {
                uint8_t biome_id = config.get_biome_id(grid_data.tiles_ids[y][x]);
                tile_row.push_back(Tile(grid_data.walkable_tiles[y][x], biome_id));
            } catch (const std::out_of_range& err) {
                tile_row.push_back(Tile(grid_data.walkable_tiles[y][x]));
            }
        }

        tiles_.emplace_back(std::move(tile_row));
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
    static std::random_device rd;
    static std::default_random_engine generator(rd());
    std::uniform_int_distribution get_random_width(0, width_ - 1);
    std::uniform_int_distribution get_random_height(0, height_ - 1);
    int x, y;
    do {
        x = get_random_width(generator);
        y = get_random_height(generator);
    } while (!is_tile_available(x, y));

    return Position(x, y);
}

bool Grid::is_tile_available(int x, int y) const {
    bool is_in_range = x >= 0 && y >= 0 && x < width_ && y < height_;

    return is_in_range && tiles_[y][x].is_walkable() && tiles_[y][x].occupant() == nullptr;
}

// TODO: seguramente se puede hacer sin crear tantos objetos
Direction Grid::closest_movement(const Position& current, const Position& target) const {
    Direction closest_direction = Direction::IDLE;
    float min_distance = current.distance_to(target);

    for (const auto& direction: directions) {
        Position position = current.move(direction);
        if (!is_tile_available(position.get_x(), position.get_y()))
            continue;

        float distance = position.distance_to(target);
        if (distance < min_distance) {
            closest_direction = direction;
            min_distance = distance;
        }
    }

    return closest_direction;
}

Direction Grid::random_movement(const Position& current) const {
    static std::random_device rd;
    static std::default_random_engine generator(rd());
    std::uniform_int_distribution<size_t> get_random_width(0, directions.size() + 4);

    size_t index = get_random_width(generator);
    Direction direction = index < directions.size() ? directions[index] : Direction::IDLE;
    Position position = current.move(direction);

    return is_tile_available(position.get_x(), position.get_y()) ? direction : Direction::IDLE;
}
