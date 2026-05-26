#include "command.h"

RequestedCommandDTO::RequestedCommandDTO(const CommandType& cmd, const Direction& direction):
        command(cmd), direction(direction), x(0), y(0) {}


RequestedCommandDTO::RequestedCommandDTO(const CommandType& cmd, const int x, const int y):
        command(cmd), direction(Direction::IDLE), x(x), y(y) {}
