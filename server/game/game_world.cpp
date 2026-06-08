#include "game_world.h"

#include <limits>
#include <utility>

#include "allies/ally.h"
#include "allies/banker.h"
#include "allies/merchant.h"
#include "allies/priest.h"
#include "server/util/server_map_loader.h"


GameWorld::GameWorld(PlayerRepository& player_repository): grid(), player_repository(player_repository) {}

void GameWorld::init() {
    ServerMapLoader loader;
    const ServerMapDataDTO map_data = loader.get_server_data();

    this->grid = Grid(map_data.width, map_data.height, map_data.grid);
    init_npc(map_data.npcs);
    init_creature();
}

const std::unordered_map<std::string, Player>& GameWorld::get_players() const { return players; }

const std::unordered_map<uint16_t, Creature>& GameWorld::get_creatures() const { return creatures; }

const std::map<std::pair<uint16_t, uint16_t>, Tile*>& GameWorld::get_lootable_tiles() const {
    return tiles_with_loot;
}

WorldUpdateStatus GameWorld::update() {
    std::vector<std::string> resurrected_players;
    for (auto& [name, player]: players) {
        player.update();
        if (player.did_just_resurrect()) {
            resurrected_players.push_back(name);
        }
    }

    remove_dead_creatures();
    std::vector<CreatureUpdateStatus> creatures_status(creatures.size());

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

        creatures_status.push_back(move_creature(creature, direction));
    }

    return WorldUpdateStatus(creatures_status, resurrected_players);
}

Direction GameWorld::next_movement(const Creature& creature) {
    const Position& current = creature.get_position();
    return creature.is_targeting_someone() ? grid.closest_movement(current, creature.get_target_position()) :
                                             grid.random_movement(current);
}

CreatureUpdateStatus GameWorld::move_creature(Creature& creature, const Direction& direction) {
    Position current = creature.get_position();

    if (direction == Direction::IDLE || !creature.can_move()) {
        return creature.update_state(current, Direction::IDLE);
    }

    Position target = current.move(direction);

    try {
        Tile& tile = grid.get_tile(target);

        if (tile.is_walkable() && tile.occupant() == nullptr) {
            grid.get_tile(current).occupy(nullptr);
            tile.occupy(&creature);

            creature.update_state(target, direction);
        } else {
            creature.update_state(current, Direction::IDLE);
        }
    } catch (const std::out_of_range& _) {
        creature.update_state(current, Direction::IDLE);
    }

    return CreatureUpdateStatus();
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
        Tile& tile = grid.get_tile(target);

        // validar colisiones
        if (not tile.is_walkable()) {
            std::cout << "[World] Jugador " << player_name << " colisionó con el mapa" << std::endl;
            return;  // o disparar evento de movimiento inválido
        }

        if (tile.occupant() != nullptr) {
            std::cout << "[World] Jugador " << player_name << " colisionó con otra entidad" << std::endl;
            return;  // o disparar evento de movimiento inválido
        }

        // efectuar movimiento
        grid.get_tile(current).occupy(nullptr);
        tile.occupy(&player);

        player.update_position(target, direction);

        // notificar el evento de movimiento
        std::cout << "[World] Jugador " << player_name << " se movió a " << target << std::endl;

    } catch (const std::out_of_range& _) {
        std::cout << "[World] Límite del mapa alcanzado" << std::endl;
    }
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
                target_tile.add_loot(target.drop());

                add_tile_if_lootable(target_tile, position);
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
    if (tile.has_loot() && !tiles_with_loot.contains(pair_position))
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

PickUpResult GameWorld::pick_up(const std::string& player_name, const Position& position) {
    if (!players.contains(player_name))
        return PickUpResult(PickUpStatus::MUST_NOT_NOTIFY);

    Player& player = players.at(player_name);

    Tile& tile = grid.get_tile(position);
    Loot loot = tile.take_loot();

    switch (loot.type) {
        case LootType::ITEM:
            return pick_item_up(player, tile, loot.item);
        case LootType::GOLD:
            return pick_gold_up(player, tile, loot.gold);
        default:
            throw std::invalid_argument("There is no loot of this type to be picked up");
    }
}

PickUpResult GameWorld::pick_item_up(Player& player, Tile& tile, uint8_t item) {
    try {
        player.acquire_item(item);
        return PickUpResult(PickUpStatus::SUCCESS);
    } catch (const InventoryFull& err) {
        tile.add_loot(item);
    } catch (const SlotFull& err) {
        tile.add_loot(item);
    }

    return PickUpResult(PickUpStatus::FAILED);
}

PickUpResult GameWorld::pick_gold_up(Player& player, Tile& tile, uint16_t gold) {
    uint16_t difference = player.add_gold(gold);
    if (difference == 0)
        return PickUpResult(PickUpStatus::SUCCESS);

    tile.add_loot(difference);
    return PickUpResult(PickUpStatus::PARTIAL);
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

void GameWorld::init_creature() {
    Position goblin_position(15, 15);
    creatures.emplace(1, Creature(0, 0, 0, goblin_position));
    grid.get_tile(goblin_position).occupy(&creatures.at(1));
}
