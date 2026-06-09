#include "command.h"

#include <cassert>


RequestedCommandDTO::RequestedCommandDTO(const CommandType& cmd, const Direction& direction):
        command(cmd),
        direction(direction),
        x(0),
        y(0),
        receiver(""),
        message(""),
        item_id(0),
        gold_amount(0),
        clan_name() {}


RequestedCommandDTO::RequestedCommandDTO(const CommandType& cmd, const int x, const int y):
        command(cmd),
        direction(Direction::IDLE),
        x(x),
        y(y),
        receiver(""),
        message(""),
        item_id(0),
        gold_amount(0),
        clan_name() {}


RequestedCommandDTO::RequestedCommandDTO(const CommandType& cmd, const std::string& receiver,
                                         const std::string& content):
        command(cmd),
        direction(Direction::IDLE),
        x(0),
        y(0),
        receiver(receiver),
        message(content),
        item_id(0),
        gold_amount(0),
        clan_name() {}


RequestedCommandDTO::RequestedCommandDTO(const CommandType& cmd):
        command(cmd),
        direction(Direction::IDLE),
        x(0),
        y(0),
        receiver(""),
        message(""),
        item_id(0),
        gold_amount(0),
        clan_name() {}


RequestedCommandDTO::RequestedCommandDTO(const CommandType& cmd, const uint8_t item_id):
        command(cmd),
        direction(Direction::IDLE),
        x(0),
        y(0),
        receiver(""),
        message(""),
        item_id(item_id),
        gold_amount(0),
        clan_name() {}


RequestedCommandDTO::RequestedCommandDTO(const CommandType& cmd, const uint16_t gold_amount):
        command(cmd),
        direction(Direction::IDLE),
        x(0),
        y(0),
        receiver(""),
        message(""),
        item_id(0),
        gold_amount(gold_amount),
        clan_name() {}

RequestedCommandDTO::RequestedCommandDTO(const CommandType& cmd, const std::string& clan_name):
        command(cmd),
        direction(Direction::IDLE),
        x(0),
        y(0),
        receiver(),
        message(),
        item_id(0),
        gold_amount(0),
        clan_name(clan_name) {
    assert(cmd == CommandType::CLAN_FOUND || cmd == CommandType::CLAN_JOIN);
}
