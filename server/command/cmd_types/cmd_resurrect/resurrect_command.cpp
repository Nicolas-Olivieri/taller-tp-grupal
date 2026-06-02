#include "resurrect_command.h"


ResurrectCommand::ResurrectCommand(const std::string& player_name):
        player_name(player_name), result(ResurrectResult::NO_RESULT) {}


void ResurrectCommand::execute(GameWorld& world) {
    result = world.resurrect_player(player_name);
    if (result == ResurrectResult::PLAYER_RESURRECTED) {
        appearance = AppearanceDTO(world.get_players().at(player_name).get_body(),
                                   world.get_players().at(player_name).get_head());
    }
}


void ResurrectCommand::build_snapshot(SnapshotBuilder& builder) {
    switch (result) {
        case ResurrectResult::PLAYER_RESURRECTED:
            builder.add_action(ActionDTO(
                    ChatMessageDTO(MessageVisibility::PRIVATE, "Sacerdote", player_name, "A sus ordenes!")));
            builder.add_action(ActionDTO(ResurrectionDTO(player_name, appearance)));
            break;
        case ResurrectResult::PLAYER_IS_ALIVE:
            builder.add_action(ActionDTO(ChatMessageDTO(MessageVisibility::PRIVATE, "Sacerdote", player_name,
                                                        "Ya estas vivo!! Avivate")));
            break;
        case ResurrectResult::PLAYER_UNBOUNDED:
            builder.add_action(ActionDTO(ChatMessageDTO(MessageVisibility::PRIVATE, "Mundo", player_name,
                                                        "No estas vinculado a ningun sacerdote")));
            break;
        case ResurrectResult::NO_RESULT:
        default:
            throw std::runtime_error("Resurrect cmd no se ejecutó correctamente");
    }
}
