#include "heal_command.h"

#include <map>


HealCommand::HealCommand(const std::string& player_name):
        player_name(player_name), result(HealResult::NO_RESULT) {}


void HealCommand::execute(GameWorld& world) { result = world.heal_player(player_name); }


void HealCommand::build_snapshot(SnapshotBuilder& builder) {
    static std::map<HealResult, std::string> result_to_message(
            {{HealResult::PLAYER_HEALED, "A sus ordenes! Toda tu vida y mana fueron recargadas"},
             {HealResult::PLAYER_IS_DEAD, "Estas muerto! Primero tenes que resucitar"}});

    if (result_to_message.contains(result)) {
        const std::string& message = result_to_message.at(result);
        builder.add_action(
                ActionDTO(ChatMessageDTO(MessageVisibility::PRIVATE, "Sacerdote", player_name, message)));
        return;
    }

    throw std::runtime_error("Heal cmd no se ejecutó correctamente");
}
