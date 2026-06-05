#include "game_world.h"

#include <utility>

#include "allies/ally.h"
#include "allies/merchant.h"
#include "allies/priest.h"


GameWorld::GameWorld(const int width, const int height): grid(width, height) {
    init_npc();
    init_creature();
}


const std::unordered_map<std::string, Player>& GameWorld::get_players() const { return players; }

const std::unordered_map<uint16_t, Creature>& GameWorld::get_creatures() const { return creatures; }

std::vector<CreatureUpdateStatus> GameWorld::update() {
    for (auto& [name, player]: players) {
        player.update();
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

    return creatures_status;
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
        const Creature& creature = it->second;

        if (!creature.is_alive()) {
            grid.get_tile(creature.get_position()).occupy(nullptr);
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

    players.erase(it);
    std::cout << "[World] Jugador " << player_name << " desconectado" << std::endl;
}

InteractResult GameWorld::interact(const std::string& player_name, const Position& position) {
    if (!players.contains(player_name))
        return InteractResult();

    Player& player = players.at(player_name);

    try {
        const Tile& target_tile = grid.get_tile(position);
        Interactive* occupant = target_tile.occupant();

        if (occupant != nullptr) {
            return occupant->interact(player);
        }
        std::cout << "[World] " << player_name << " golpeó al aire" << std::endl;
    } catch (const std::out_of_range&) {
        // Golpeó el borde del mapa
    }
    return InteractResult();
}


ResurrectResult GameWorld::resurrect_player(const std::string& player_name) {
    return execute_ally_action(player_name, AllyActionPayload(AllyAction::RESURRECT)).resurrect;
}


HealResult GameWorld::heal_player(const std::string& player_name) {
    return execute_ally_action(player_name, AllyActionPayload(AllyAction::HEAL)).heal;
}


ListItemsResult GameWorld::list_ally_items(const std::string& player_name) {
    return execute_ally_action(player_name, AllyActionPayload(AllyAction::LIST_ITEMS)).list_items;
}

BuyResult GameWorld::buy_item(const std::string& player_name, const uint8_t item_id) {
    return execute_ally_action(player_name, AllyActionPayload(AllyAction::BUY, item_id)).buy;
}

SellResult GameWorld::sell_item(const std::string& player_name, const uint8_t item_id) {
    return execute_ally_action(player_name, AllyActionPayload(AllyAction::SELL, item_id)).sell;
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
            // TODO: Implementar lógica de detener al jugador un tiempo proporcional a la distancia con el
            //  sacerdote más cercano, para luego hacer que aparezca junto al mismo
            return AllyExecuteResult(ResurrectResult(ResurrectStatus::PLAYER_RESURRECTED, AllyType::PRIEST));
        }

        std::cout << "[World] Jugador " << player_name << " no tiene vinculado a ningún aliado" << std::endl;
        return AllyExecuteResult(false);
    }

    return ally->execute(player, payload);
}


void GameWorld::init_npc() {
    Position priest_position(10, 10);
    auto priest = std::make_unique<Priest>(priest_position);
    grid.get_tile(priest_position).occupy(priest.get());
    allies.push_back(std::move(priest));

    Position merchant_position(15, 10);
    auto merchant = std::make_unique<Merchant>(merchant_position);
    grid.get_tile(merchant_position).occupy(merchant.get());
    allies.push_back(std::move(merchant));
}

void GameWorld::init_creature() {
    Position goblin_position(30, 30);
    creatures.emplace(1, Creature(0, 0, 0, goblin_position));
    grid.get_tile(goblin_position).occupy(&creatures.at(1));
}
