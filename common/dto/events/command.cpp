#include "command.h"

RequestedCommandDTO::RequestedCommandDTO(const CommandType& cmd, const Direction& direction):
        command(cmd), direction(direction) {}
