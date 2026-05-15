#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include <iostream>
#include <string>
#include <unordered_map>

#include "common/direction.h"

#include "player.h"
#include "position.h"


class GameWorld {
private:
    std::unordered_map<std::string, Player> players;

    std::unordered_map<Direction, Position> movements = {
            {Direction::UP, Position(0, -1)},
            {Direction::DOWN, Position(0, 1)},
            {Direction::RIGHT, Position(1, 0)},
            {Direction::LEFT, Position(-1, 0)},
    };

public:
    void move_player(const std::string& player_name, Direction direction);

    void add_player(const std::string& player_name, const Position& position);

private:
    Position calculate_position(const Position& current, Direction direction);

    bool is_tile_walkable(const Position& current);

    bool is_tile_occupied_by_entity(const Position& current);
};


#endif  // GAME_WORLD_H
