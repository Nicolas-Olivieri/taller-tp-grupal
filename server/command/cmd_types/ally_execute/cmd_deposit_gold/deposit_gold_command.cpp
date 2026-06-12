#include "deposit_gold_command.h"

#include <map>


DepositGoldCommand::DepositGoldCommand(const std::string& player_name, const uint16_t gold_amount):
        player_name(player_name), gold_amount(gold_amount), result() {}


void DepositGoldCommand::execute(GameWorld& world) { result = world.deposit_gold(player_name, gold_amount); }


void DepositGoldCommand::build_snapshot(SnapshotBuilder& builder) {
    if (result.status == DepositGoldStatus::PLAYER_UNBOUNDED) {
        const std::string& error_message = "Hablale a un banquero para poder depositar oro";
        builder.add_action(ActionDTO(ChatMessageDTO(MessageType::ERROR, player_name, error_message)));
        return;
    }

    static std::map<AllyType, std::string> ally_type_to_string({
            {AllyType::PRIEST, "Sacerdote"},
            {AllyType::MERCHANT, "Comerciante"},
            {AllyType::BANKER, "Banquero"},
    });

    if (not ally_type_to_string.contains(result.ally)) {
        throw std::runtime_error("DepositGoldCommand recibió un NPC aliado desconocido");
    }

    static std::map<DepositGoldStatus, std::string> result_to_message({
            {DepositGoldStatus::GOLD_DEPOSITED, "Tu oro ha sido depositado en el banco"},
            {DepositGoldStatus::NOT_ENOUGH_GOLD, "No te alcanza el oro para depositar esa cantidad"},
            {DepositGoldStatus::BANK_FULL, "El banco ya está al máximo"},
            {DepositGoldStatus::GHOST_FAIL, "Perdón, no atiendo fantasmas"},
            {DepositGoldStatus::ACTION_NOT_ACCEPTED, "Perdón, yo no puedo hacer eso"},
    });

    if (not result_to_message.contains(result.status)) {
        throw std::runtime_error("DepositGoldCommand recibió un resultado incorrecto");
    }

    const std::string& sender = ally_type_to_string.at(result.ally);
    const std::string& content = result_to_message.at(result.status);
    builder.add_action(ActionDTO(ChatMessageDTO(MessageType::ALLY, sender, player_name, content)));
}
