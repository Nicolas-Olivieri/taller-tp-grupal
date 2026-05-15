#include "game_world.h"


Position GameWorld::calculate_position(const Position& current,
                                       const Direction direction) {
    return current + movements.at(direction);
}


bool GameWorld::is_tile_walkable(const Position& current) {
    // todo: chequear a partir de la Tile en la posición current
    return current != Position(0, -2);
}


bool GameWorld::is_tile_occupied_by_entity(const Position& current) {
    // colisiones con otros jugadores
    for (const auto& pair: players) {
        if (pair.second.get_position() == current) {
            return true;
        }
    }
    // colisiones con NPC...
    return false;
}


void GameWorld::move_player(const std::string& player_name,
                            const Direction direction) {
    // buscar al jugador
    const auto it = players.find(player_name);
    if (it == players.end()) {
        return;
    }

    auto& player = it->second;

    std::cout << "[World] Jugador " << player_name << " intentando moverse "
              << std::endl;

    // calcular coordenada destino
    const Position target =
            calculate_position(player.get_position(), direction);

    // validar colisiones
    if (not is_tile_walkable(target)) {
        std::cout << "[World] Jugador " << player_name << " colisionó "
              << std::endl;
        return;  // o disparar evento de movimiento inválido
    }

    if (is_tile_occupied_by_entity(target)) {
        return;
    }

    // efectuar movimiento
    player.move(target);

    // notificar el evento de movimiento
    std::cout << "[World] Jugador " << player_name << " se movió a " << target
              << std::endl;
}


void GameWorld::add_player(const std::string& player_name,
                           const Position& position) {
    players.insert({player_name, Player(player_name, position)});
    std::cout << "[World] Jugador " << player_name << " creado en " << position
              << std::endl;
}
