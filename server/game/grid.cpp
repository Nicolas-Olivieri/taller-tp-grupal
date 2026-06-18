#include "grid.h"

#include <algorithm>
#include <utility>

#include "server/util/calculator.h"

Grid::Grid(): width_(0), height_(0) {}

Grid::Grid(const int width, const int height, const GridMatrixDTO& grid_data):
        width_(width),
        height_(height),
        directions({Direction::DOWN, Direction::RIGHT, Direction::LEFT, Direction::UP}) {
    for (const auto& row: grid_data.tiles_info) {
        std::vector<Tile> tile_row;
        tile_row.reserve(row.size());

        std::ranges::transform(row, std::back_inserter(tile_row), [](const auto& tile_value) {
            return Tile(tile_value.walkable, tile_value.floor);
        });

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
    GameConfig& config = GameConfig::get();

    static std::random_device rd;
    static std::default_random_engine generator(rd());
    std::uniform_int_distribution get_random_width(0, width_ - 1);
    std::uniform_int_distribution get_random_height(0, height_ - 1);
    int x, y;
    do {
        x = get_random_width(generator);
        y = get_random_height(generator);
    } while (!is_tile_available(x, y) || !config.has_biome_associated(tiles_[y][x].floor) ||
             config.get_biome_id(tiles_[y][x].floor) != SAFE_ZONE_FLOOR);

    return Position(x, y);
}

Position Grid::spawn_near(const std::vector<Position>& positions) const {
    std::vector<Position> near_positions;

    for (const auto& position: positions) {
        add_near_positions(near_positions, position.get_x(), position.get_y());
    }

    if (near_positions.empty())
        throw std::runtime_error("There are no positions near any player to spawn a creature");

    return Calculator::random_choice(near_positions);
}

void Grid::add_near_positions(std::vector<Position>& near_positions, uint16_t pos_x, uint16_t pos_y) const {
    GameConfig& config = GameConfig::get();

    for (uint16_t y = std::max(0, pos_y - config.get_grid_constants().max_near_factor);
         y < std::min(height_, pos_y + config.get_grid_constants().max_near_factor); y++) {
        for (uint16_t x = std::max(0, pos_x - config.get_grid_constants().max_near_factor);
             x < std::min(width_, pos_x + config.get_grid_constants().max_near_factor); x++) {
            uint16_t distance_x = std::abs(x - pos_x);
            uint16_t distance_y = std::abs(y - pos_y);

            uint16_t current_distance = std::max(distance_x, distance_y);

            if (current_distance >= config.get_grid_constants().min_near_factor && is_tile_available(x, y)) {
                near_positions.push_back(Position(x, y));
            }
        }
    }
}

bool Grid::is_tile_available(int x, int y) const {
    bool is_in_range = x >= 0 && y >= 0 && x < width_ && y < height_;

    return is_in_range && tiles_[y][x].is_walkable() && tiles_[y][x].occupant() == nullptr;
}

// TODO: seguramente se puede hacer sin crear tantos objetos
Direction Grid::closest_movement(const Position& current, const Position& target) const {
    Direction closest_direction = Direction::IDLE;
    float min_distance = MAXFLOAT;

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
    std::uniform_int_distribution<size_t> get_random_width(
            0, directions.size() + GameConfig::get().get_grid_constants().roam_idle_weight);

    size_t index = get_random_width(generator);
    Direction direction = index < directions.size() ? directions[index] : Direction::IDLE;
    Position position = current.move(direction);

    return is_tile_available(position.get_x(), position.get_y()) ? direction : Direction::IDLE;
}
