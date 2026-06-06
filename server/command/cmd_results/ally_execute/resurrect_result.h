#ifndef RESURRECT_RESULT_H
#define RESURRECT_RESULT_H

#include "common/npc_type.h"


enum class ResurrectStatus {
    PLAYER_RESURRECTED,
    PLAYER_IS_ALIVE,
    RESURRECTION_PENDING,
    ACTION_NOT_ACCEPTED,
    PLAYER_UNBOUNDED,
    NO_RESULT,
};


struct ResurrectResult {
    ResurrectStatus status{ResurrectStatus::NO_RESULT};
    AllyType ally{AllyType::NO_ALLY};
};


#endif  // RESURRECT_RESULT_H
