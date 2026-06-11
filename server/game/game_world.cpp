#include "game_world.h"

#include <cassert>
#include <limits>
#include <utility>

#include "allies/ally.h"
#include "allies/banker.h"
#include "allies/merchant.h"
#include "allies/priest.h"
#include "server/command/cmd_results/unequip_item/unequip_item_result.h"
#include "server/command/cmd_results/use_item/use_item_result.h"
#include "server/game/clan/clan.h"
#include "server/util/server_map_loader.h"

#define MAX_CREATURE_AMOUNT 10  // TODO: toml

GameWorld::GameWorld(PlayerRepository& player_repository):
        grid(), current_creature_id(0), player_repository(player_repository) {}

void GameWorld::init() {
    ServerMapLoader loader;
    const ServerMapDataDTO map_data = loader.get_server_data();

    this->grid = Grid(map_data.width, map_data.height, map_data.grid);
    init_npc(map_data.npcs);
    load_clans();
}

const std::unordered_map<std::string, Player>& GameWorld::get_players() const { return players; }

const std::unordered_map<uint16_t, Creature>& GameWorld::get_creatures() const { return creatures; }

const std::map<std::pair<uint16_t, uint16_t>, Tile*>& GameWorld::get_lootable_tiles() const {
    return tiles_with_loot;
}

void GameWorld::exchange_position(const Position& old_position, const Position& new_position,
                                  Interactive* occupant) {
    grid.get_tile(old_position).occupy(nullptr);
    grid.get_tile(new_position).occupy(occupant);
}

WorldUpdateStatus GameWorld::update() {
    std::vector<std::string> resurrected_players;
    for (auto& [name, player]: players) {
        const Position previous_position = player.get_position();
        player.update();

        if (player.did_just_resurrect()) {
            resurrected_players.push_back(name);
            const Position& resurrect_position = player.get_position();

            exchange_position(previous_position, resurrect_position, &player);
        }
    }

    remove_dead_creatures();

    if (creatures.size() < MAX_CREATURE_AMOUNT)
        spawn_random_creature();

    std::vector<CreatureUpdate> creatures_status;
    creatures_status.reserve(creatures.size());

    for (auto& [id, creature]: creatures) {
        creature.update();

        Direction direction = next_movement(creature);

        if (!creature.is_targeting_someone()) {
            for (auto& [name, player]: players) {
                if (player.is_alive() && creature.can_target(player.get_position())) {
                    creature.target_player(player);
                    break;
                }
            }
        }

        CreatureUpdate creature_update = manage_creature_attack(creature);
        creatures_status.push_back(creature_update);

        if (creature_update.status == CreatureStatus::MOVING)
            move_creature(creature, direction);

        creature.update_state();
    }

    return WorldUpdateStatus(creatures_status, resurrected_players);
}

Direction GameWorld::next_movement(const Creature& creature) {
    const Position& current = creature.get_position();
    return creature.is_targeting_someone() ? grid.closest_movement(current, creature.get_target_position()) :
                                             grid.random_movement(current);
}

CreatureUpdate GameWorld::manage_creature_attack(Creature& creature) {
    if (!creature.can_reach_target())
        return CreatureUpdate(CreatureStatus::MOVING);

    if (creature.is_targeting_someone() && creature.can_attack()) {
        CreatureUpdate creature_update = creature.attack_player();

        Player& target = players.at(creature.get_target_name());
        if (!target.is_alive())
            drop_player_items(target);

        return creature_update;
    }

    return CreatureUpdate(CreatureStatus::WAITING);
}

void GameWorld::move_creature(Creature& creature, const Direction& direction) {
    Position current = creature.get_position();

    if (!creature.can_move() || direction == Direction::IDLE)
        return;

    Position target = current.move(direction);

    if (grid.is_tile_available(target.get_x(), target.get_y())) {
        exchange_position(current, target, &creature);
        creature.update_position(target, direction);
    }
}

void GameWorld::move_player(const std::string& player_name, const Direction direction) {
    // buscar al jugador
    const auto it = players.find(player_name);
    if (it == players.end()) {
        return;
    }

    auto& player = it->second;

    if (not player.can_move()) {
        return;
    }

    const Position current = player.get_position();

    // calcular coordenada destino
    const Position target = current.move(direction);

    try {
        if (grid.is_tile_available(target.get_x(), target.get_y())) {
            exchange_position(current, target, &player);
            player.update_position(target, direction);
        }

    } catch (const std::out_of_range& _) {}
}

void GameWorld::add_player(const std::string& player_name, const PlayerData& data) {
    const auto it = emplace_player(player_name, data);
    grid.get_tile(it->second.get_position()).occupy(&(it->second));
    std::cout << "[World] Jugador " << player_name << " creado en " << it->second.get_position() << " con "
              << it->second.get_stats().health.get_current() << " puntos de salud" << std::endl;
}

std::unordered_map<std::string, Player>::iterator GameWorld::emplace_player(const std::string& player_name,
                                                                            const PlayerData& data) {
    if (data.has_played_before) {
        auto [it, success] = players.emplace(player_name, Player(player_name, data));
        return it;
    } else {
        Position random_position(grid.spawn());
        auto [it, success] = players.emplace(player_name, Player(player_name, data, random_position));
        return it;
    }
}

void GameWorld::remove_dead_creatures() {
    for (auto it = creatures.begin(); it != creatures.end();) {
        Creature& creature = it->second;

        if (!creature.is_alive()) {
            const Position& position = creature.get_position();
            Tile& tile = grid.get_tile(position);
            tile.occupy(nullptr);
            tile.add_loot(creature.drop());

            add_tile_if_lootable(tile, position);

            it = creatures.erase(it);
        } else {
            it++;
        }
    }
}

void GameWorld::spawn_random_creature() {
    // TODO: cambiar este método para considerar biomas
    uint8_t variation_id = Calculator::random_number(0, 2);
    const VariationData& variation = GameConfig::get().get_variation(variation_id);

    uint8_t creature_id = Calculator::random_choice(variation.compatible_races);

    Position spawn_position = grid.spawn();
    uint16_t id = get_next_creature_id();

    creatures.emplace(id, Creature(creature_id, variation_id, spawn_position));
    grid.get_tile(spawn_position).occupy(&creatures.at(id));
}

uint16_t GameWorld::get_next_creature_id() {
    assert(MAX_CREATURE_AMOUNT < UINT16_MAX);

    // Aprovecha el overflow de UINT16_MAX -> 0 para volver a usar los ids que se liberaron
    while (creatures.contains(current_creature_id)) current_creature_id++;

    uint16_t id = current_creature_id++;

    return id;
}

void GameWorld::remove_player(const std::string& player_name) {
    const auto it = players.find(player_name);
    if (it == players.end()) {
        return;
    }
    grid.get_tile(it->second.get_position()).occupy(nullptr);

    for (auto& [id, creature]: creatures) {
        if (creature.is_targeting(it->second))
            creature.stop_targeting();
    }

    player_repository.save_progress(it->second);
    player_repository.desconnect(player_name);
    players.erase(it);
    std::cout << "[World] Jugador " << player_name << " desconectado" << std::endl;
}

InteractResult GameWorld::interact(const std::string& player_name, const Position& position) {
    if (!players.contains(player_name))
        return InteractResult();

    Player& player = players.at(player_name);

    try {
        Tile& target_tile = grid.get_tile(position);
        Interactive* occupant = target_tile.occupant();

        if (occupant != nullptr) {
            InteractResult result = occupant->interact(player);

            if (result.attack.was_killed) {
                Player& target = players.at(result.attack.player_attacked);
                drop_and_add(target, target_tile);
            }

            return result;
        }
        std::cout << "[World] " << player_name << " golpeó al aire" << std::endl;
    } catch (const std::out_of_range&) {
        // Golpeó el borde del mapa
    }
    return InteractResult();
}

void GameWorld::add_tile_if_lootable(Tile& tile, const Position& position) {
    std::pair<uint16_t, uint16_t> pair_position = {position.get_x(), position.get_y()};
    if (not tile.get_loot().empty() && !tiles_with_loot.contains(pair_position))
        tiles_with_loot.insert({pair_position, &tile});
}

ResurrectResult GameWorld::resurrect_player(const std::string& player_name) {
    return execute_ally_action(player_name, AllyActionPayload(AllyAction::RESURRECT)).resurrect;
}


HealResult GameWorld::heal_player(const std::string& player_name) {
    return execute_ally_action(player_name, AllyActionPayload(AllyAction::HEAL)).heal;
}


std::unique_ptr<ListOutcome> GameWorld::list_ally_items(const std::string& player_name) {
    const AllyExecuteResult result =
            execute_ally_action(player_name, AllyActionPayload(AllyAction::LIST_ITEMS));

    if (result.list_bank.was_player_bounded) {
        auto bank_vault = std::make_unique<BankVaultOutcome>();
        bank_vault->ally = result.list_bank.ally;
        bank_vault->gold = result.list_bank.gold;
        bank_vault->items = result.list_bank.items;
        return bank_vault;
    }

    if (result.list_items.was_player_bounded) {
        auto vendor_list = std::make_unique<VendorListOutcome>();
        vendor_list->ally = result.list_items.ally;
        vendor_list->items = result.list_items.items;
        return vendor_list;
    }

    return std::make_unique<PlayerUnboundOutcome>();
}

BuyResult GameWorld::buy_item(const std::string& player_name, const uint8_t item_id) {
    return execute_ally_action(player_name, AllyActionPayload(AllyAction::BUY, item_id)).buy;
}

SellResult GameWorld::sell_item(const std::string& player_name, const uint8_t item_id) {
    return execute_ally_action(player_name, AllyActionPayload(AllyAction::SELL, item_id)).sell;
}

DepositItemResult GameWorld::deposit_item(const std::string& player_name, const uint8_t item_id) {
    return execute_ally_action(player_name, AllyActionPayload(AllyAction::DEPOSIT_ITEM, item_id))
            .deposit_item;
}

WithdrawItemResult GameWorld::withdraw_item(const std::string& player_name, const uint8_t item_id) {
    return execute_ally_action(player_name, AllyActionPayload(AllyAction::WITHDRAW_ITEM, item_id))
            .withdraw_item;
}

DepositGoldResult GameWorld::deposit_gold(const std::string& player_name, const uint16_t gold_amount) {
    return execute_ally_action(player_name, AllyActionPayload(AllyAction::DEPOSIT_GOLD, gold_amount))
            .deposit_gold;
}

WithdrawGoldResult GameWorld::withdraw_gold(const std::string& player_name, const uint16_t gold_amount) {
    return execute_ally_action(player_name, AllyActionPayload(AllyAction::WITHDRAW_GOLD, gold_amount))
            .withdraw_gold;
}

PickUpResult GameWorld::pick_up(const std::string& player_name) {
    if (!players.contains(player_name))
        return PickUpResult();

    Player& player = players.at(player_name);

    if (!player.is_alive())
        return PickUpResult(PickUpStatus::GHOST_FAIL);

    const Position& position = player.get_position();

    Tile& tile = grid.get_tile(position);
    if (tile.get_loot().empty())
        return PickUpResult(PickUpStatus(PickUpStatus::NO_LOOT));

    const Loot& loot = tile.get_loot().top();

    PickUpResult result = loot.type == LootType::ITEM ? pick_item_up(player, tile, loot.item) :
                                                        pick_gold_up(player, tile, loot.gold);

    if (result.status == PickUpStatus::SUCCESS && tile.get_loot().empty())
        tiles_with_loot.extract({position.get_x(), position.get_y()});

    return result;
}

PickUpResult GameWorld::pick_item_up(Player& player, Tile& tile, uint8_t item) {
    try {
        player.acquire_item(item);
        tile.get_loot().pop();
        return PickUpResult(PickUpStatus::SUCCESS);
    } catch (const InventoryFull& err) {
    } catch (const SlotFull& err) {}

    return PickUpResult(PickUpStatus::NOT_ENOUGH_SPACE);
}

PickUpResult GameWorld::pick_gold_up(Player& player, Tile& tile, uint16_t gold) {
    uint16_t previous_gold = player.get_safe_gold() + player.get_excess_gold();
    player.add_gold(gold);
    uint16_t current_gold = player.get_safe_gold() + player.get_excess_gold();

    tile.get_loot().pop();

    if (current_gold - previous_gold == gold) {
        return PickUpResult(PickUpStatus::SUCCESS);
    } else if (current_gold - previous_gold == 0) {
        return PickUpResult(PickUpStatus::NOT_ENOUGH_SPACE);
    }

    return PickUpResult(PickUpStatus::GOLD_OVERFLOW);
}

UseItemResult GameWorld::use_item(const std::string& player_name, const uint8_t item_id) {
    if (not players.contains(player_name))
        return UseItemResult();

    Player& player = players.at(player_name);

    // No debería ocurrir porque los jugadores pierden todo al morir, pero igual
    if (not player.is_alive())
        return UseItemResult(UseItemStatus::GHOST_FAIL);

    try {
        player.use_item(item_id);
        return UseItemResult(UseItemStatus::SUCCESS);

    } catch (const ItemNotOwned&) {
        return UseItemResult(UseItemStatus::FAILED);
    }
}

UnequipItemResult GameWorld::unequip_item(const std::string& player_name, const uint8_t item_id) {
    if (not players.contains(player_name))
        return UnequipItemResult();

    Player& player = players.at(player_name);
    try {
        player.unequip_item(item_id);
        return UnequipItemResult(UnequipItemStatus::SUCCESS);

    } catch (const InventoryFull&) {
        return UnequipItemResult(UnequipItemStatus::FAILED);
    }
}

DropItemResult GameWorld::drop_item(const std::string& player_name, const uint8_t item_id) {
    if (not players.contains(player_name))
        return DropItemResult();

    Player& player = players.at(player_name);
    const Position& position = player.get_position();
    try {
        player.drop_item(item_id);
        Tile& tile = grid.get_tile(position);
        tile.add_loot(Loot(item_id));
        add_tile_if_lootable(tile, position);
        return DropItemResult(DropItemStatus::SUCCESS);

    } catch (const ItemNotOwned&) {
        return DropItemResult(DropItemStatus::ITEM_NOT_OWNED);

    } catch (const ItemEquipped&) {
        return DropItemResult(DropItemStatus::ITEM_EQUIPPED);
    }
}

AllyExecuteResult GameWorld::execute_ally_action(const std::string& player_name,
                                                 const AllyActionPayload& payload) {
    if (not players.contains(player_name)) {
        return AllyExecuteResult(true);
    }

    Player& player = players.at(player_name);

    const auto ally = player.get_bound_ally();
    if (ally == nullptr) {
        if (payload.action == AllyAction::RESURRECT) {
            return resurrect_unbounded_player(player);
        }

        std::cout << "[World] Jugador " << player_name << " no tiene vinculado a ningún aliado" << std::endl;
        return AllyExecuteResult(false);
    }

    return ally->execute(player, payload);
}


AllyExecuteResult GameWorld::resurrect_unbounded_player(Player& player) const {
    if (player.is_alive()) {
        return AllyExecuteResult(ResurrectResult(ResurrectStatus::PLAYER_IS_ALIVE, AllyType::PRIEST));
    }

    const Ally* closest_priest = find_closest_priest(player);
    if (closest_priest != nullptr) {
        return start_delayed_resurrection(player, closest_priest);
    }

    return AllyExecuteResult(ResurrectResult(ResurrectStatus::NO_RESULT, AllyType::PRIEST));
}


const Ally* GameWorld::find_closest_priest(const Player& player) const {
    const Ally* closest_priest = nullptr;
    double min_distance = std::numeric_limits<double>::max();
    const Position& player_position = player.get_position();
    for (const auto& ally_ptr: allies) {
        if (ally_ptr.get()->get_type() == AllyType::PRIEST) {
            const Position& priest_position = ally_ptr.get()->get_position();
            const double distance = player_position.distance_to(priest_position);
            if (distance < min_distance) {
                min_distance = distance;
                closest_priest = ally_ptr.get();
            }
        }
    }

    return closest_priest;
}


AllyExecuteResult GameWorld::start_delayed_resurrection(Player& player, const Ally* priest) const {
    // TODO: El factor de proporcionalidad debe venir del TOML
    constexpr double time_factor = 2;
    const double distance = player.get_position().distance_to(priest->get_position());
    const double wait_time = distance * time_factor;
    player.start_delayed_resurrection(wait_time, priest->get_position());
    return AllyExecuteResult(ResurrectResult(ResurrectStatus::RESURRECTION_PENDING, AllyType::PRIEST));
}


void GameWorld::init_npc(const std::vector<AllyInfoDTO>& npcs) {
    for (const auto& npc: npcs) {
        Position position(npc.x, npc.y + 1);
        std::unique_ptr<Ally> ally;

        switch (npc.type) {
            case AllyType::PRIEST:
                ally = std::make_unique<Priest>(position);
                break;
            case AllyType::MERCHANT:
                ally = std::make_unique<Merchant>(position);
                break;
            case AllyType::BANKER:
                ally = std::make_unique<Banker>(position);
                break;
            case AllyType::NO_ALLY:
            default:
                break;
        }

        if (ally) {
            grid.get_tile(position).occupy(ally.get());
            allies.push_back(std::move(ally));
        }
    }
}

void GameWorld::drop_player_items(Player& player) {
    Tile& target_tile = grid.get_tile(player.get_position());
    drop_and_add(player, target_tile);
}

void GameWorld::drop_and_add(Player& player, Tile& tile) {
    tile.add_loot(player.drop());

    add_tile_if_lootable(tile, player.get_position());
}

FoundClanResult GameWorld::found_clan(const std::string& player_name, const std::string& clan_name) {
    if (not players.contains(player_name)) {
        return FoundClanResult::NO_RESULT;
    }

    Player& player = players.at(player_name);

    if (clans.contains(clan_name))
        return FoundClanResult::CLAN_ALREADY_EXISTS;

    const std::string current_clan = player.get_clan_name();

    if (not current_clan.empty())
        return FoundClanResult::ALREADY_IN_CLAN;

    const uint8_t current_level = player.get_stats().experience.get_level();
    if (current_level < Clan::MIN_LEVEL_REQUIRED_TO_FOUND_CLAN)
        return FoundClanResult::NOT_ENOUGH_LEVEL;

    if (clan_name.size() > CLAN_NAME)
        return FoundClanResult::CLAN_NAME_LONG;

    Clan new_clan(clan_name, player_name);

    clans.insert({clan_name, std::move(new_clan)});
    player.found_clan(clan_name);
    return FoundClanResult::SUCCESS;
}

ClanActionResult GameWorld::execute_clan_action(const ClanActionPayload& payload) {
    if (not players.contains(payload.player_name)) {
        return ClanActionResult();
    }

    Player& player = players.at(payload.player_name);
    const std::string clan_name = player.get_clan_name();

    if (clan_name.empty())
        return ClanActionResult(ClanActionStatus::NOT_IN_CLAN);

    if ((not players.contains(payload.other_player)) and (not payload.other_player.empty()))
        return ClanActionResult(ClanActionStatus::NOT_A_PLAYER);

    assert(clans.contains(clan_name));

    Clan& clan = clans.at(clan_name);

    ClanActionResult result = clan.execute(payload);

    if (result.status == ClanActionStatus::SUCCESS) {
        switch (payload.type) {
            case ClanActionType::ACCEPT: {
                Player& player_accepted = players.at(payload.other_player);
                if (not player_accepted.get_clan_name().empty()) {
                    clan.remove(payload.other_player);
                    return ClanActionResult(ClanActionStatus::PLAYER_HAS_CLAN);
                }
                player_accepted.join_clan(clan_name);
                break;
            }
            case ClanActionType::LEAVE:
                player.leave_clan();
                break;
            case ClanActionType::KICK: {
                Player& player_kicked = players.at(payload.other_player);
                player_kicked.leave_clan();
                break;
            }
            case ClanActionType::BAN: {
                Player& player_banned = players.at(payload.other_player);
                if (player_banned.get_clan_name() == clan_name)
                    player_banned.leave_clan();
                break;
            }
            default:
                break;
        }
    }

    return result;
}

JoinClanResult GameWorld::join_clan(const std::string& player_name, const std::string& clan_name) {
    if (not players.contains(player_name)) {
        return JoinClanResult::NO_RESULT;
    }

    const Player& player = players.at(player_name);

    if (not player.get_clan_name().empty())
        return JoinClanResult::ALREADY_IN_CLAN;

    if (!clans.contains(clan_name))
        return JoinClanResult::CLAN_NOT_FOUND;

    clans.at(clan_name).recv_join_request(player_name);
    return JoinClanResult::SUCCESS;
}

void GameWorld::load_clans() {
    std::vector<ClanData> saved_clans = player_repository.get_saved_clans();

    for (const auto& data: saved_clans) {
        Clan clan(data);
        clans.insert({data.clan_name, clan});
    }
}

void GameWorld::cheat_player_xp(const std::string& player_name, const uint8_t level) {
    if (not players.contains(player_name)) {
        return;
    }

    Player& player = players.at(player_name);
    player.set_xp_level(level);
}
