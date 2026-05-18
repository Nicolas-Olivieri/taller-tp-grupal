#ifndef COMMAND_H
#define COMMAND_H

#include <string>

#include "common/direction.h"

#include "message.h"

enum class CommandType : uint8_t { MOVE };

// No implementa ProtocolMessageDTO porque este no viaja por red, se construye a
// partir de los EventDTO
struct RequestedCommandDTO {
    CommandType command;
    std::string player_name;  // no viaja por red
    Direction direction;

    /* TODO: agregar los siguientes al implementar los correspondientes comandos
    item_id
    other_player
    message
    clan_name
    x
    y
    */

    RequestedCommandDTO(const CommandType& cmd, const Direction& direction);
};

#endif  // COMMAND_H
