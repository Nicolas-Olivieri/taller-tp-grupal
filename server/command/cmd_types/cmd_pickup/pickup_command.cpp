#include "pickup_command.h"

#include <string>

PickUpCommand::PickUpCommand(const std::string& player_name, uint16_t x, uint16_t y):
        player_name(player_name), position(x, y) {}

void PickUpCommand::execute(GameWorld& world) { result = world.pick_up(player_name, position); }

void PickUpCommand::build_snapshot(SnapshotBuilder& builder) {
    switch (result.status) {
        case PickUpStatus::SUCCESS:
            builder.add_action(ActionDTO(
                    ChatMessageDTO(MessageType::SYSTEM, player_name, "El botín se agrego a tu inventario")));
            break;
        case PickUpStatus::FAILED:
            builder.add_action(ActionDTO(ChatMessageDTO(MessageType::ERROR, player_name,
                                                        "No tienes espacio para llevarte el botin")));
            break;
        case PickUpStatus::MUST_NOT_NOTIFY:
            break;
        default:
            throw std::invalid_argument("There is no defined way to add this pick up result to the snapshot");
    }
}
