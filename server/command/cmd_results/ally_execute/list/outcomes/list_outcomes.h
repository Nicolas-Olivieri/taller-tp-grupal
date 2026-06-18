#ifndef LIST_OUTCOMES_H
#define LIST_OUTCOMES_H

#include "common/npc_type.h"


struct ListOutcome {
    AllyType ally;

    ListOutcome();

    virtual ~ListOutcome() = default;
};


#endif  // LIST_OUTCOMES_H
