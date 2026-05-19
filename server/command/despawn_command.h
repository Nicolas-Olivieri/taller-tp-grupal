#ifndef DESPAWN_COMMAND_H
#define DESPAWN_COMMAND_H

#include <string>

#include "command.h"


class DespawnCommand: public Command {
private:
    std::string player_name;

public:
    explicit DespawnCommand(const std::string& player_name);

    void execute(GameWorld& world) override;

    void broadcast(EventBroadcaster& broadcaster) override;
};


#endif  // DESPAWN_COMMAND_H
