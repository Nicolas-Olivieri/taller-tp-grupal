#include "get_item.h"

GetItemCommand::GetItemCommand(const std::string& player_name, uint8_t item):
        player_name(player_name), item(item) {}

void GetItemCommand::execute(GameWorld& world) { world.cheat_get_item(player_name, item); }

void GetItemCommand::build_snapshot(SnapshotBuilder& builder) {
    builder.add_action(ActionDTO(ChatMessageDTO(MessageType::SYSTEM, player_name,
                                                "De ser capaz, el item fue agregado a tu inventario")));
}
