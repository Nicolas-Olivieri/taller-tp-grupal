#include "resurrect_command.h"

#include <cassert>


ResurrectCommand::ResurrectCommand(const std::string& player_name):
        player_name(player_name), result(ResurrectResult::NO_RESULT) {}


void ResurrectCommand::execute(GameWorld& world) {
    result = world.resurrect_player(player_name);
    if (result == ResurrectResult::PLAYER_RESURRECTED) {
        const uint8_t body = world.get_players().at(player_name).get_body();
        const uint8_t head = world.get_players().at(player_name).get_head();
        std::cout << "[ResurrectCommand] body: " << static_cast<int>(body) << std::endl;
        std::cout << "[ResurrectCommand] head: " << static_cast<int>(head) << std::endl;
        appearance = AppearanceDTO(body, head);
    }
}


void ResurrectCommand::build_snapshot(SnapshotBuilder& builder) {
    switch (result) {
        case ResurrectResult::PLAYER_RESURRECTED:
            builder.add_action(ActionDTO(ResurrectionDTO(player_name, AppearanceDTO())));
            break;
        case ResurrectResult::PLAYER_IS_ALIVE:
            builder.add_action(ActionDTO(ChatMessageDTO(MessageVisibility::PRIVATE, "Sacerdote", player_name,
                                                        "Ya estás vivo!! Avivate")));
            break;
        case ResurrectResult::PLAYER_UNBOUNDED:
            builder.add_action(ActionDTO(ChatMessageDTO(MessageVisibility::PRIVATE, "Mundo", player_name,
                                                        "No estás vinculado a ningún sacerdote")));
            break;
        case ResurrectResult::NO_RESULT:
        default:
            throw std::runtime_error("Resurrect cmd no se ejecutó correctamente");
    }
}
