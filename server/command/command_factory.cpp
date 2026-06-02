#include "command_factory.h"

#include "cmd_types/cmd_chatmessage/chatmessage_command.h"
#include "cmd_types/cmd_heal/heal_command.h"
#include "cmd_types/cmd_interact/interact_command.h"
#include "cmd_types/cmd_move/move_command.h"
#include "cmd_types/cmd_resurrect/resurrect_command.h"


CommandFactory::CommandFactory(const std::string& player_name): player_name(player_name) {}


std::unique_ptr<Command> CommandFactory::create(const RequestedCommandDTO& dto) const {
    switch (dto.command) {
        case CommandType::MOVE:
            return std::make_unique<MoveCommand>(player_name, dto.direction);

        case CommandType::INTERACT:
            return std::make_unique<InteractCommand>(player_name, dto.x, dto.y);

        case CommandType::CHAT:
            return std::make_unique<ChatMessageCommand>(player_name, dto.receiver, dto.message);

        case CommandType::RESURRECT:
            return std::make_unique<ResurrectCommand>(player_name);

        case CommandType::HEAL:
            return std::make_unique<HealCommand>(player_name);

        default:
            throw std::invalid_argument("Comando desconocido");
    }
}
