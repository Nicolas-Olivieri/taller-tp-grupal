#include "game_world.h"


GameWorld::GameWorld(const int width, const int height): grid(width, height) {}


std::unordered_map<std::string, Player> GameWorld::get_players() const { return players; }


void GameWorld::move_player(const std::string& player_name, const Direction direction) {
    // buscar al jugador
    const auto it = players.find(player_name);
    if (it == players.end()) {
        return;
    }

    auto& player = it->second;
    const Position current = player.get_position();

    std::cout << "[World] Jugador " << player_name << " intentando moverse " << std::endl;

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

        player.move(target, direction);

        // notificar el evento de movimiento
        std::cout << "[World] Jugador " << player_name << " se movió a " << target << std::endl;

    } catch (const std::out_of_range& _) {
        std::cout << "[World] Límite del mapa alcanzado" << std::endl;
    }
}


void GameWorld::add_player(const std::string& player_name, const Position& position) {
    auto [it, success] = players.emplace(player_name, Player(player_name, position));
    grid.get_tile(position).occupy(&(it->second));
    std::cout << "[World] Jugador " << player_name << " creado en " << position << std::endl;
}


void GameWorld::add_player(const std::string& player_name) { add_player(player_name, grid.spawn()); }


void GameWorld::remove_player(const std::string& player_name) {
    const auto it = players.find(player_name);
    if (it == players.end()) {
        return;
    }
    players.erase(it);
    std::cout << "[World] Jugador " << player_name << " desconectado" << std::endl;
}
