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
    DEPOSIT_GOLD,
    WITHDRAW_GOLD,
    PICKUP,
    USE_ITEM,
    DROP_ITEM,
    UNEQUIP_ITEM,
    CLAN_FOUND,
    CLAN_JOIN,
    CLAN_REQUEST_RESPONSE,
    CLAN_REMOVE_PLAYER,
    CLAN_LEAVE,
    CLAN_REVIEW,
    CHEAT_XP,
    CHEAT_GOLD,
    CHEAT_DEATH
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

    // OJO si se cambia esto de uint_8 a algo más cuidado que tmb se usa para enviar el nivel de experiencia
    uint8_t item_id;

    uint16_t gold_amount;

    std::string clan_name;
    bool command_selector;


    // TODO: Refactorizar los constructores para que determinen el CommandType a partir de los parámetros
    RequestedCommandDTO(const CommandType& cmd, const Direction& direction);

    RequestedCommandDTO(const CommandType& cmd, int x, int y);

    RequestedCommandDTO(const CommandType& cmd, const std::string& receiver, const std::string& content);

    explicit RequestedCommandDTO(const CommandType& cmd);

    RequestedCommandDTO(const CommandType& cmd, uint8_t item_id);

    RequestedCommandDTO(const CommandType& cmd, uint16_t gold_amount);

    RequestedCommandDTO(const CommandType& cmd, const std::string& clan_name);

    RequestedCommandDTO(const CommandType& cmd, const std::string& player_name, bool command_selector);
};

#endif  // REQUESTEDCOMMANDDTO_H
