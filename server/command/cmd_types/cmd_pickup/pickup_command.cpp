#include "pickup_command.h"

#include <string>

PickUpCommand::PickUpCommand(const std::string& player_name): player_name(player_name) {}

void PickUpCommand::execute(GameWorld& world) { result = world.pick_up(player_name); }

void PickUpCommand::build_snapshot(SnapshotBuilder& builder) {
    if (result.status == PickUpStatus::MUST_NOT_NOTIFY)
        return;

    builder.add_action(ActionDTO(create_message()));
}

ChatMessageDTO PickUpCommand::create_message() {
    switch (result.status) {
        case PickUpStatus::SUCCESS:
            return ChatMessageDTO(MessageType::SYSTEM, player_name, "El botin se agrego a tu inventario");
        case PickUpStatus::NOT_ENOUGH_SPACE:
            return ChatMessageDTO(MessageType::ERROR, player_name,
                                  "No tienes espacio para llevarte el botin");
        case PickUpStatus::GHOST_FAIL:
            return ChatMessageDTO(MessageType::ERROR, player_name,
                                  "No puedes levantar cosas del piso como fantasma");
        case PickUpStatus::GOLD_OVERFLOW:
            return ChatMessageDTO(MessageType::SYSTEM, player_name,
                                  "Tomaste el oro, pero tus bolsillos se llenaron");
        case PickUpStatus::NO_LOOT:
            return ChatMessageDTO(MessageType::ERROR, player_name, "No hay nada en el piso para levantar");

        case PickUpStatus::MUST_NOT_NOTIFY:
        default:
            throw std::invalid_argument("There is no defined way to add this pick up result to the snapshot");
    }
}
