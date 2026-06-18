#include "set_experience_command.h"

SetExperienceCommand::SetExperienceCommand(const std::string& player_name, uint8_t level):
        player_name(player_name), level(level) {}

void SetExperienceCommand::execute(GameWorld& world) {
    if (level > 0)
        world.cheat_player_xp(player_name, level);
}

void SetExperienceCommand::build_snapshot(SnapshotBuilder& builder) {
    if (level > 0)
        builder.add_action(ActionDTO(
                ChatMessageDTO(MessageType::SYSTEM, player_name, std::format("Ahora eres nivel {}", level))));
}
