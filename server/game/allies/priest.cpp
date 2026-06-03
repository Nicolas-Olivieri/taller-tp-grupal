#include "priest.h"

#include "server/game/player/player.h"


Priest::Priest(const Position& position): Ally(position) {}


AllyExecuteResult Priest::execute(Player& player, const AllyActionPayload& payload) {
    switch (payload.action) {
        case AllyAction::HEAL:
            return handle_heal(player);

        case AllyAction::RESURRECT:
            return handle_resurrect(player);

        case AllyAction::BUY:
            // TODO: Implementar la lógica de comprar hechizos y pociones

        default:
            // El sacerdote ignora los comandos de venta
            break;
    }

    return AllyExecuteResult();
}


AllyType Priest::get_type() const { return AllyType::PRIEST; }


AllyExecuteResult Priest::handle_heal(Player& player) {
    if (player.is_alive()) {
        std::cout << "[Priest] El jugador fue curado" << std::endl;
        player.heal();
        return AllyExecuteResult(HealResult::PLAYER_HEALED);
    }

    std::cout << "[Priest] El jugador está muerto" << std::endl;
    return AllyExecuteResult(HealResult::PLAYER_IS_DEAD);
}


AllyExecuteResult Priest::handle_resurrect(Player& player) {
    if (not player.is_alive()) {
        std::cout << "[Priest] El jugador fue resucitado" << std::endl;
        player.heal();
        return AllyExecuteResult(ResurrectResult::PLAYER_RESURRECTED);
    }

    std::cout << "[Priest] El jugador ya está vivo" << std::endl;
    return AllyExecuteResult(ResurrectResult::PLAYER_IS_ALIVE);
}
