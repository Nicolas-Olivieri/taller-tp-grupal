#ifndef REQUESTEDCOMMANDDTO_H
#define REQUESTEDCOMMANDDTO_H

#include <string>

#include "common/direction.h"

enum class CommandType : uint8_t {
    MOVE,
    INTERACT,
    CHAT,
    RESURRECT,
    HEAL,
    LIST_ITEMS,
    BUY_ITEM,
    SELL_ITEM,
    DEPOSIT_ITEM,
    WITHDRAW_ITEM,
};

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

    uint8_t item_id;

    /* TODO: agregar los siguientes al implementar los correspondientes comandos
    clan_name
    */

    // TODO: Refactorizar los constructores para que determinen el CommandType a partir de los parámetros
    RequestedCommandDTO(const CommandType& cmd, const Direction& direction);

    RequestedCommandDTO(const CommandType& cmd, int x, int y);

    RequestedCommandDTO(const CommandType& cmd, const std::string& receiver, const std::string& content);

    explicit RequestedCommandDTO(const CommandType& cmd);

    RequestedCommandDTO(const CommandType& cmd, uint8_t item_id);
};

#endif  // REQUESTEDCOMMANDDTO_H
