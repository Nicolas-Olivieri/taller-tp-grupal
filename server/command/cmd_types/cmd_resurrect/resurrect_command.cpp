#include "resurrect_command.h"

#include <map>

ResurrectCommand::ResurrectCommand(const std::string& player_name): player_name(player_name), result() {}


void ResurrectCommand::execute(GameWorld& world) {
    result = world.resurrect_player(player_name);
    if (result.status == ResurrectStatus::PLAYER_RESURRECTED) {
        appearance = AppearanceDTO(world.get_players().at(player_name).get_body(),
                                   world.get_players().at(player_name).get_head());
    }
}


void ResurrectCommand::build_snapshot(SnapshotBuilder& builder) {
    static std::map<AllyType, std::string> ally_type_to_string({
            {AllyType::PRIEST, "Sacerdote"},
            {AllyType::MERCHANT, "Comerciante"},
            {AllyType::BANKER, "Banquero"},
    });

    if (not ally_type_to_string.contains(result.ally)) {
        throw std::runtime_error("ResurrectCommand recibió un NPC aliado desconocido");
    }

    static std::map<ResurrectStatus, std::string> result_to_message(
            {{ResurrectStatus::PLAYER_RESURRECTED, "A sus ordenes! Bienvenido de nuevo a la vida"},
             {ResurrectStatus::PLAYER_IS_ALIVE, "Ya estas vivo! Avivate"},
             {ResurrectStatus::ACTION_NOT_ACCEPTED, "Perdon, yo no puedo hacer eso"}});

    if (not result_to_message.contains(result.status)) {
        throw std::runtime_error("ResurrectCommand recibió un resultado incorrecto");
    }

    const std::string& sender = ally_type_to_string.at(result.ally);
    const std::string& content = result_to_message.at(result.status);
    builder.add_action(ActionDTO(ChatMessageDTO(MessageType::ALLY, sender, player_name, content)));

    if (result.status == ResurrectStatus::PLAYER_RESURRECTED) {
        builder.add_action(ActionDTO(ResurrectionDTO(player_name, appearance)));
    }
}
