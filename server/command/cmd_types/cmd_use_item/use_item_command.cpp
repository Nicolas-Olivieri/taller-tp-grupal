#include "use_item_command.h"


UseItemCommand::UseItemCommand(const std::string& player_name, const uint8_t item_id):
        player_name(player_name), item_id(item_id), result() {}


void UseItemCommand::execute(GameWorld& world) { result = world.use_item(player_name, item_id); }


void UseItemCommand::build_snapshot(SnapshotBuilder& builder) {
    switch (result.status) {
        case UseItemStatus::SUCCESS:
            builder.add_action(ActionDTO(ChatMessageDTO(MessageType::SYSTEM, player_name, "Usaste el item")));
            break;

        case UseItemStatus::FAILED:
            builder.add_action(
                    ActionDTO(ChatMessageDTO(MessageType::ERROR, player_name, "No tienes ese item")));
            break;

        case UseItemStatus::MUST_NOT_NOTIFY:
            break;

        default:
            throw std::invalid_argument("UseItemCommand recibió un resultado incorrecto");
    }
}
