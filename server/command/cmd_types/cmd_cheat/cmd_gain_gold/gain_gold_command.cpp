#include "gain_gold_command.h"

GainGoldCommand::GainGoldCommand(const std::string& player_name, uint16_t gold_amount) : player_name(player_name), gold_amount(gold_amount) {}

void GainGoldCommand::execute(GameWorld& world) {
    world.cheat_player_gold(player_name, gold_amount);
}

void GainGoldCommand::build_snapshot(SnapshotBuilder& builder) {
    builder.add_action(ActionDTO(ChatMessageDTO(MessageType::SYSTEM, player_name, std::format("Se te agregaron {} de oro", gold_amount))));
}