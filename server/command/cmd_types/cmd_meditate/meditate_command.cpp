#include "meditate_command.h"

#include <map>

MeditateCommand::MeditateCommand(const std::string& player_name): player_name(player_name) {}


void MeditateCommand::execute(GameWorld& world) { result = world.meditate(player_name); }


void MeditateCommand::build_snapshot(SnapshotBuilder& builder) {
    if (result.status == MeditateStatus::MUST_NOT_NOTIFY)
        return;

    static const std::map<MeditateStatus, std::string> result_to_message(
            {{MeditateStatus::SUCCESS, "Meditando..."},
             {MeditateStatus::GHOST_FAIL, "No podés meditar siendo un fantasma"},
             {MeditateStatus::ARCHETYPE_FAIL, "Tu clase no es capaz de meditar"}});


    if (not result_to_message.contains(result.status))
        throw std::runtime_error("MeditateCommand recibió un resultado incorrecto");

    const std::string& content = result_to_message.at(result.status);
    builder.add_action(ActionDTO(ChatMessageDTO(MessageType::SYSTEM, player_name, content)));

    if (result.status == MeditateStatus::SUCCESS)
        builder.add_action(ActionDTO(MeditationDTO(player_name)));
}
