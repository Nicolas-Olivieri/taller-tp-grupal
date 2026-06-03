#include "priest.h"

#include "server/game/player/player.h"


Priest::Priest(const Position& position): Ally(position) {}


AllyExecuteResult Priest::execute(Player& player, const AllyActionPayload& payload) {
    switch (payload.action) {
        case AllyAction::HEAL:
            if (player.is_alive()) {
                std::cout << "[Priest] El jugador fue curado" << std::endl;
                player.heal();
            } else {
                std::cout << "[Priest] El jugador está muerto" << std::endl;
            }
            // TODO: Notificar mediante un evento / mensaje
            break;

        case AllyAction::RESURRECT:
            if (not player.is_alive()) {
                std::cout << "[Priest] El jugador fue resucitado" << std::endl;
                player.heal();
                return AllyExecuteResult(ResurrectResult::PLAYER_RESURRECTED);
            }

            std::cout << "[Priest] El jugador ya está vivo" << std::endl;
            return AllyExecuteResult(ResurrectResult::PLAYER_IS_ALIVE);

        case AllyAction::BUY:
            // TODO: Implementar la lógica de comprar hechizos y pociones

        default:
            // El sacerdote ignora los comandos de venta
            break;
    }
    return AllyExecuteResult(ResurrectResult::NO_RESULT);
}


AllyType Priest::get_type() const { return AllyType::PRIEST; }
