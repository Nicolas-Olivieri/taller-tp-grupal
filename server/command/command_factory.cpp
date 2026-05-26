#include "command_factory.h"

#include "interact_command.h"
#include "move_command.h"


CommandFactory::CommandFactory(const std::string& player_name): player_name(player_name) {}


std::unique_ptr<Command> CommandFactory::create(const RequestedCommandDTO& dto) const {
    switch (dto.command) {
        case CommandType::MOVE:
            return std::make_unique<MoveCommand>(player_name, dto.direction);

        case CommandType::INTERACT:
            return std::make_unique<InteractCommand>(player_name, dto.x, dto.y);

        default:
            throw std::invalid_argument("Comando desconocido");
    }
}
