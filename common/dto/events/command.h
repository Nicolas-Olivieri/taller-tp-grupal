#ifndef REQUESTEDCOMMANDDTO_H
#define REQUESTEDCOMMANDDTO_H

#include <string>

#include "common/direction.h"

enum class CommandType : uint8_t { MOVE, INTERACT, CHAT };

// No implementa ProtocolMessageDTO porque este no viaja por red, se construye a
// partir de los EventDTO
struct RequestedCommandDTO {
    CommandType command;
    std::string player_name;  // no viaja por red
    Direction direction;

    int x;
    int y;

    std::string receiver;
    std::string message;
    /* TODO: agregar los siguientes al implementar los correspondientes comandos
    item_id
    clan_name
    */

    // TODO: Refactorizar los constructores para que determinen el CommandType a partir de los parámetros
    RequestedCommandDTO(const CommandType& cmd, const Direction& direction);

    RequestedCommandDTO(const CommandType& cmd, int x, int y);

    RequestedCommandDTO(const CommandType& cmd, const std::string& receiver, const std::string& content);
};

#endif  // REQUESTEDCOMMANDDTO_H
