#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "allies/ally_action_payload.h"
#include "common/direction.h"
#include "player/player.h"
#include "server/command/cmd_results/ally_execute/list_outcomes.h"
#include "server/command/cmd_results/ally_execute/resurrect_result.h"

#include "grid.h"
#include "position.h"


class GameWorld {
private:
    Grid grid;

    std::unordered_map<std::string, Player> players;

    std::vector<std::unique_ptr<Ally>> allies;

public:
    explicit GameWorld(int width, int height);

    std::unordered_map<std::string, Player> get_players() const;

    void update();

    void move_player(const std::string& player_name, Direction direction);

    void add_player(const std::string& player_name, const PlayerData& data);

    std::unordered_map<std::string, Player>::iterator emplace_player(const std::string& player_name,
                                                                     const PlayerData& data);

    void remove_player(const std::string& player_name);

    InteractResult interact(const std::string& player_name, const Position& position);

    ResurrectResult resurrect_player(const std::string& player_name);

    HealResult heal_player(const std::string& player_name);

    std::unique_ptr<ListOutcome> list_ally_items(const std::string& player_name);

    BuyResult buy_item(const std::string& player_name, uint8_t item_id);

    SellResult sell_item(const std::string& player_name, uint8_t item_id);

    DepositItemResult deposit_item(const std::string& player_name, uint8_t item_id);

    WithdrawItemResult withdraw_item(const std::string& player_name, uint8_t item_id);

    DepositGoldResult deposit_gold(const std::string& player_name, uint16_t gold_amount);

    WithdrawGoldResult withdraw_gold(const std::string& player_name, uint16_t gold_amount);

private:
    AllyExecuteResult execute_ally_action(const std::string& player_name, const AllyActionPayload& payload);

    void init_npc();
};


#endif  // GAME_WORLD_H
