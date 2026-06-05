#include "withdraw_item_command.h"

#include <map>


WithdrawItemCommand::WithdrawItemCommand(const std::string& player_name, const uint8_t item_id):
        player_name(player_name), item_id(item_id), result() {}


void WithdrawItemCommand::execute(GameWorld& world) { result = world.withdraw_item(player_name, item_id); }


void WithdrawItemCommand::build_snapshot(SnapshotBuilder& builder) {
    if (result.status == WithdrawItemStatus::PLAYER_UNBOUNDED) {
        const std::string& error_message = "Hablale a un banquero para poder retirar un item";
        builder.add_action(ActionDTO(ChatMessageDTO(MessageType::ERROR, player_name, error_message)));
        return;
    }

    static std::map<AllyType, std::string> ally_type_to_string({
            {AllyType::PRIEST, "Sacerdote"},
            {AllyType::MERCHANT, "Comerciante"},
            {AllyType::BANKER, "Banquero"},
    });

    if (not ally_type_to_string.contains(result.ally)) {
        throw std::runtime_error("WithdrawItemCommand recibió un NPC aliado desconocido");
    }

    static std::map<WithdrawItemStatus, std::string> result_to_message({
            {WithdrawItemStatus::ITEM_WITHDRAWN, "Tu item ha sido retirado del banco, gracias por confiar"},
            {WithdrawItemStatus::ITEM_NOT_IN_BANK,
             "Estas tratando de retirar un item que no tenes en el banco"},
            {WithdrawItemStatus::INVENTORY_FULL,
             "Tu inventario ya esta al maximo, deshazte de un tipo item primero"},
            {WithdrawItemStatus::ACTION_NOT_ACCEPTED, "Perdon, yo no puedo hacer eso"},
    });

    if (not result_to_message.contains(result.status)) {
        throw std::runtime_error("WithdrawItemCommand recibió un resultado incorrecto");
    }

    const std::string& sender = ally_type_to_string.at(result.ally);
    const std::string& content = result_to_message.at(result.status);
    builder.add_action(ActionDTO(ChatMessageDTO(MessageType::ALLY, sender, player_name, content)));
}
