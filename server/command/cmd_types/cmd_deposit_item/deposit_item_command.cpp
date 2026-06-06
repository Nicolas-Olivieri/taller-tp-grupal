#include "deposit_item_command.h"

#include <map>


DepositItemCommand::DepositItemCommand(const std::string& player_name, const uint8_t item_id):
        player_name(player_name), item_id(item_id), result() {}


void DepositItemCommand::execute(GameWorld& world) { result = world.deposit_item(player_name, item_id); }


void DepositItemCommand::build_snapshot(SnapshotBuilder& builder) {
    if (result.status == DepositItemStatus::PLAYER_UNBOUNDED) {
        const std::string& error_message = "Hablale a un banquero para poder depositar un item";
        builder.add_action(ActionDTO(ChatMessageDTO(MessageType::ERROR, player_name, error_message)));
        return;
    }

    static std::map<AllyType, std::string> ally_type_to_string({
            {AllyType::PRIEST, "Sacerdote"},
            {AllyType::MERCHANT, "Comerciante"},
            {AllyType::BANKER, "Banquero"},
    });

    if (not ally_type_to_string.contains(result.ally)) {
        throw std::runtime_error("DepositItemCommand recibió un NPC aliado desconocido");
    }

    static std::map<DepositItemStatus, std::string> result_to_message({
            {DepositItemStatus::ITEM_DEPOSITED, "Tu item ha sido depositado en el banco"},
            {DepositItemStatus::ITEM_NOT_OWNED,
             "Estas tratando de depositar un item que no tenes en tu inventario"},
            {DepositItemStatus::ITEM_EQUIPPED, "Estas tratando de depositar un item que tenes equipado"},
            {DepositItemStatus::BANK_FULL, "El banco ya esta al maximo, retira un tipo item primero"},
            {DepositItemStatus::ACTION_NOT_ACCEPTED, "Perdon, yo no puedo hacer eso"},
    });

    if (not result_to_message.contains(result.status)) {
        throw std::runtime_error("DepositItemCommand recibió un resultado incorrecto");
    }

    const std::string& sender = ally_type_to_string.at(result.ally);
    const std::string& content = result_to_message.at(result.status);
    builder.add_action(ActionDTO(ChatMessageDTO(MessageType::ALLY, sender, player_name, content)));
}
