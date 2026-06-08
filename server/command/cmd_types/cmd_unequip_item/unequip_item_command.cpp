#include "unequip_item_command.h"


UnequipItemCommand::UnequipItemCommand(const std::string& player_name, const uint8_t item_id):
        player_name(player_name), item_id(item_id), result() {}


void UnequipItemCommand::execute(GameWorld& world) { result = world.unequip_item(player_name, item_id); }


void UnequipItemCommand::build_snapshot(SnapshotBuilder& builder) {
    switch (result.status) {
        case UnequipItemStatus::SUCCESS:
            builder.add_action(
                    ActionDTO(ChatMessageDTO(MessageType::SYSTEM, player_name, "Te desequipaste el item")));
            break;

        case UnequipItemStatus::FAILED:
            builder.add_action(ActionDTO(ChatMessageDTO(MessageType::ERROR, player_name,
                                                        "No tienes espacio para desequiparte el item")));
            break;

        case UnequipItemStatus::MUST_NOT_NOTIFY:
            break;

        default:
            throw std::invalid_argument("UnequipItemCommand recibió un resultado incorrecto");
    }
}
