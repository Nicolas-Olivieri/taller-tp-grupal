#include "game_world.h"


GameWorld::GameWorld(const int width, const int height): grid(width, height) {}


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

        player.update_position(target, direction);

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
    grid.get_tile(it->second.get_position()).occupy(nullptr);
    players.erase(it);
    std::cout << "[World] Jugador " << player_name << " desconectado" << std::endl;
}


// TODO: Esta versión no tiene en cuenta el arma equipada
// TODO: Validar que el jugador que intenta atacar no está muerto
void GameWorld::attack(const std::string& player_name, const Position& position) {
    const auto it = players.find(player_name);
    if (it == players.end()) {
        return;
    }

    Player& attacker = it->second;

    if (std::abs(attacker.get_position().get_x() - position.get_x()) > 1 or
        std::abs(attacker.get_position().get_y() - position.get_y()) > 1) {
        std::cout << "[World] El objetivo está demasiado lejos" << std::endl;
        return;
    }

    if (not attacker.can_attack()) {
        std::cout << "[World] Jugador " << player_name << " intentó atacar pero está en cooldown"
                  << std::endl;
        return;
    }

    if (position == attacker.get_position()) {
        std::cout << "[World] Jugador " << player_name << " intentó atacarse a sí mismo" << std::endl;
        return;
    }

    try {
        const Tile& target_tile = grid.get_tile(position);
        Interactive* occupant = target_tile.occupant();

        if (occupant != nullptr) {
            attacker.attack();
            if (occupant->interact(attacker)) {
                std::cout << "[World] " << player_name << " mató a la entidad" << std::endl;
            }

        } else {
            std::cout << "[World] " << player_name << " golpeó al aire" << std::endl;
        }

    } catch (const std::out_of_range&) {
        // Golpeó el borde del mapa
    }
}
