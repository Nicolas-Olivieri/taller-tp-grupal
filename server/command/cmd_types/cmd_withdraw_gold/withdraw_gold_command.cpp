#include "withdraw_gold_command.h"

#include <map>


WithdrawGoldCommand::WithdrawGoldCommand(const std::string& player_name, const uint16_t gold_amount):
        player_name(player_name), gold_amount(gold_amount), result() {}


void WithdrawGoldCommand::execute(GameWorld& world) {
    result = world.withdraw_gold(player_name, gold_amount);
}


void WithdrawGoldCommand::build_snapshot(SnapshotBuilder& builder) {
    if (result.status == WithdrawGoldStatus::PLAYER_UNBOUNDED) {
        const std::string& error_message = "Hablale a un banquero para poder retirar oro";
        builder.add_action(ActionDTO(ChatMessageDTO(MessageType::ERROR, player_name, error_message)));
        return;
    }

    static std::map<AllyType, std::string> ally_type_to_string({
            {AllyType::PRIEST, "Sacerdote"},
            {AllyType::MERCHANT, "Comerciante"},
            {AllyType::BANKER, "Banquero"},
    });

    if (not ally_type_to_string.contains(result.ally)) {
        throw std::runtime_error("WithdrawGoldCommand recibió un NPC aliado desconocido");
    }

    static std::map<WithdrawGoldStatus, std::string> result_to_message({
            {WithdrawGoldStatus::GOLD_WITHDRAWN, "Has retirado del banco la cantidad de oro pedida"},
            {WithdrawGoldStatus::NOT_ENOUGH_GOLD,
             "Estas tratando de retirar mas oro del que tenes en el banco"},
            {WithdrawGoldStatus::INVENTORY_FULL, "Tu monedero esta al maximo, gasta un poco de oro primero"},
            {WithdrawGoldStatus::ACTION_NOT_ACCEPTED, "Perdon, yo no puedo hacer eso"},
    });

    if (not result_to_message.contains(result.status)) {
        throw std::runtime_error("WithdrawGoldCommand recibió un resultado incorrecto");
    }

    const std::string& sender = ally_type_to_string.at(result.ally);
    const std::string& content = result_to_message.at(result.status);
    builder.add_action(ActionDTO(ChatMessageDTO(MessageType::ALLY, sender, player_name, content)));
}
