#include "drop_item_command.h"


DropItemCommand::DropItemCommand(const std::string& player_name, const uint8_t item_id):
        player_name(player_name), item_id(item_id), result() {}


void DropItemCommand::execute(GameWorld& world) { result = world.drop_item(player_name, item_id); }


void DropItemCommand::build_snapshot(SnapshotBuilder& builder) {
    switch (result.status) {
        case DropItemStatus::SUCCESS:
            builder.add_action(
                    ActionDTO(ChatMessageDTO(MessageType::SYSTEM, player_name, "Tiraste el item al suelo")));
            break;

        case DropItemStatus::ITEM_NOT_OWNED:
            builder.add_action(ActionDTO(
                    ChatMessageDTO(MessageType::ERROR, player_name,
                                   "Estas tratando de tirar un item que no tenes en tu inventario")));
            break;

        case DropItemStatus::ITEM_EQUIPPED:
            builder.add_action(ActionDTO(ChatMessageDTO(
                    MessageType::ERROR, player_name, "Estas tratando de tirar un item que tenes equipado")));
            break;

        case DropItemStatus::MUST_NOT_NOTIFY:
            break;

        default:
            throw std::invalid_argument("DropItemCommand recibió un resultado incorrecto");
    }
}
