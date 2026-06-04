#include "game_world.h"

#include <cassert>
#include <utility>

#include "allies/ally.h"
#include "allies/priest.h"


GameWorld::GameWorld(const int width, const int height): grid(width, height) { init_npc(); }


std::unordered_map<std::string, Player> GameWorld::get_players() const { return players; }


void GameWorld::update() {
    for (auto& [name, player]: players) {
        player.update();
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

void GameWorld::remove_player(const std::string& player_name) {
    const auto it = players.find(player_name);
    if (it == players.end()) {
        return;
    }
    grid.get_tile(it->second.get_position()).occupy(nullptr);
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
        std::cout << "[World] Jugador " << player_name << " no tiene vinculado a ningún aliado" << std::endl;
        // TODO: Revisar cómo devolver el resultado de PLAYER_UNBOUNDED (justamente /resucitar es el único
        //  comando que puede realizarse sin estar vinculado a un sacerdote).
        //  Mientras que para las otras acciones no se debería devolver algo relacionado al ResurrectResult.
        return AllyExecuteResult(false);
    }

    const AllyExecuteResult result = ally->execute(player, payload);
    return result;
}


void GameWorld::init_npc() {
    Position priest_position(10, 10);
    auto priest = std::make_unique<Priest>(priest_position);
    grid.get_tile(priest_position).occupy(priest.get());
    allies.push_back(std::move(priest));
}
