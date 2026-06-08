#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "allies/ally_action_payload.h"
#include "common/direction.h"
#include "common/dto/snapshot/map/server_map_data.h"
#include "creatures/creature.h"
#include "player/player.h"
#include "server/command/cmd_results/ally_execute/list_outcomes.h"
#include "server/command/cmd_results/ally_execute/resurrect_result.h"
#include "server/command/cmd_results/pickup/pickup_result.h"
#include "server/command/cmd_results/use_item/use_item_result.h"
#include "server/persistance/playerrepository.h"

#include "grid.h"
#include "position.h"
#include "world_update_status.h"


class GameWorld {
private:
    Grid grid;

    std::unordered_map<std::string, Player> players;

    std::unordered_map<uint16_t, Creature> creatures;

    std::map<std::pair<uint16_t, uint16_t>, Tile*> tiles_with_loot;

    std::vector<std::unique_ptr<Ally>> allies;

    PlayerRepository& player_repository;

public:
    explicit GameWorld(PlayerRepository& pLayer_repository);

    void init();

    const std::unordered_map<std::string, Player>& get_players() const;

    const std::unordered_map<uint16_t, Creature>& get_creatures() const;

    const std::map<std::pair<uint16_t, uint16_t>, Tile*>& get_lootable_tiles() const;

    WorldUpdateStatus update();

    CreatureUpdateStatus move_creature(Creature& creature, const Direction& direction);

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

    PickUpResult pick_up(const std::string& player_name, const Position& position);

    UseItemResult use_item(const std::string& player_name, uint8_t item_id);

private:
    AllyExecuteResult execute_ally_action(const std::string& player_name, const AllyActionPayload& payload);

    AllyExecuteResult resurrect_unbounded_player(Player& player) const;

    const Ally* find_closest_priest(const Player& player) const;

    AllyExecuteResult start_delayed_resurrection(Player& player, const Ally* priest) const;

    Direction next_movement(const Creature& creature);

    PickUpResult pick_item_up(Player& player, Tile& tile, uint8_t item);

    PickUpResult pick_gold_up(Player& player, Tile& tile, uint16_t gold);

    void init_creature();

    void remove_dead_creatures();

    void init_npc(const std::vector<AllyInfoDTO>& npcs);

    void add_tile_if_lootable(Tile& tile, const Position& position);
};


#endif  // GAME_WORLD_H
