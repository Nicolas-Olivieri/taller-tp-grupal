#include "set_experience_command.h"

#include "server/config/game_config.h"

SetExperienceCommand::SetExperienceCommand(const std::string& player_name, uint8_t level):
        player_name(player_name), level(level) {}

void SetExperienceCommand::execute(GameWorld& world) {
    if (is_valid_level(level))
        world.cheat_player_xp(player_name, level);
}

void SetExperienceCommand::build_snapshot(SnapshotBuilder& builder) {
    if (is_valid_level(level))
        builder.add_action(ActionDTO(
                ChatMessageDTO(MessageType::SYSTEM, player_name, std::format("Ahora eres nivel {}", level))));
}

bool SetExperienceCommand::is_valid_level(uint8_t level) {
    return level >= GameConfig::get().get_killables_constants().min_level &&
           level <= GameConfig::get().get_killables_constants().max_level;
}
