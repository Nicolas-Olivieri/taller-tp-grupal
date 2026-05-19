#ifndef COMMAND_FACTORY_H
#define COMMAND_FACTORY_H

#include <memory>
#include <string>

#include "common/dto/events/command.h"

#include "command.h"


class CommandFactory {
private:
    std::string player_name;

public:
    explicit CommandFactory(const std::string& player_name);

    std::unique_ptr<Command> create(const RequestedCommandDTO& dto) const;
};


#endif  // COMMAND_FACTORY_H
