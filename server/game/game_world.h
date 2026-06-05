#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "allies/ally_action_payload.h"
#include "common/direction.h"
#include "creatures/creature.h"
#include "player/player.h"
#include "server/command/cmd_results/ally_execute/resurrect_result.h"

#include "grid.h"
#include "position.h"


class GameWorld {
private:
    Grid grid;

    std::unordered_map<std::string, Player> players;

    std::unordered_map<uint16_t, Creature> creatures;

    std::vector<std::unique_ptr<Ally>> allies;

public:
    explicit GameWorld(int width, int height);

    const std::unordered_map<std::string, Player>& get_players() const;

    const std::unordered_map<uint16_t, Creature>& get_creatures() const;

    std::vector<CreatureUpdateStatus> update();

    CreatureUpdateStatus move_creature(Creature& creature, const Direction& direction);

    void move_player(const std::string& player_name, Direction direction);

    void add_player(const std::string& player_name, const PlayerData& data);

    std::unordered_map<std::string, Player>::iterator emplace_player(const std::string& player_name,
                                                                     const PlayerData& data);

    void remove_player(const std::string& player_name);

    InteractResult interact(const std::string& player_name, const Position& position);

    ResurrectResult resurrect_player(const std::string& player_name);

    HealResult heal_player(const std::string& player_name);

    ListItemsResult list_ally_items(const std::string& player_name);

private:
    AllyExecuteResult execute_ally_action(const std::string& player_name, const AllyActionPayload& payload);

    Direction next_movement(const Creature& creature);

    void init_npc();

    void init_creature();

    void remove_dead_creatures();
};


#endif  // GAME_WORLD_H
