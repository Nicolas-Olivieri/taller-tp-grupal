#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include <iostream>
#include <string>
#include <unordered_map>

#include "common/direction.h"
#include "player/player.h"

#include "grid.h"
#include "position.h"


class GameWorld {
private:
    Grid grid;

    std::unordered_map<std::string, Player> players;

public:
    explicit GameWorld(int width, int height);

    std::unordered_map<std::string, Player> get_players() const;

    void update();

    void move_player(const std::string& player_name, Direction direction);

    void add_player(const std::string& player_name, const PlayerData& data);

    std::unordered_map<std::string, Player>::iterator emplace_player(const std::string& player_name,
                                                                     const PlayerData& data);

    void remove_player(const std::string& player_name);

    void interact(const std::string& player_name, const Position& position);
};


#endif  // GAME_WORLD_H
