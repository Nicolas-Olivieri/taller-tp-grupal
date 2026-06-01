#include "command.h"

RequestedCommandDTO::RequestedCommandDTO(const CommandType& cmd, const Direction& direction):
        command(cmd), direction(direction), x(0), y(0), receiver(""), message("") {}


RequestedCommandDTO::RequestedCommandDTO(const CommandType& cmd, const int x, const int y):
        command(cmd), direction(Direction::IDLE), x(x), y(y), receiver(""), message("") {}

RequestedCommandDTO::RequestedCommandDTO(const CommandType& cmd, const std::string& receiver,
                                         const std::string& content):
        command(cmd), direction(Direction::IDLE), x(0), y(0), receiver(receiver), message(content) {}
