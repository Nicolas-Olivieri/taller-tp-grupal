#ifndef REQUESTEDCOMMANDDTO_H
#define REQUESTEDCOMMANDDTO_H

#include <string>

#include "common/direction.h"

enum class CommandType : uint8_t { MOVE, INTERACT };

// No implementa ProtocolMessageDTO porque este no viaja por red, se construye a
// partir de los EventDTO
struct RequestedCommandDTO {
    CommandType command;
    std::string player_name;  // no viaja por red
    Direction direction;

    int x;
    int y;

    /* TODO: agregar los siguientes al implementar los correspondientes comandos
    item_id
    other_player
    message
    clan_name
    */

    // TODO: Refactorizar los constructores para que determinen el CommandType a partir de los parámetros
    RequestedCommandDTO(const CommandType& cmd, const Direction& direction);

    RequestedCommandDTO(const CommandType& cmd, int x, int y);
};

#endif  // REQUESTEDCOMMANDDTO_H
