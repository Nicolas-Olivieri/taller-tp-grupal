#ifndef PERSISTANCECOMMAND_H
#define PERSISTANCECOMMAND_H

#include "server/persistance/playerdatabase.h"
#include "server/persistance/playerindex.h"

class PlayerPersistanceCommand {
public:
    virtual void execute(PlayerDataBase& database, PlayerIndex& index) = 0;
    virtual ~PlayerPersistanceCommand() = 0;
};

#endif  // PERSISTANCECOMMAND_H
