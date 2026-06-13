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
    ResurrectStatus status;
    AllyType ally;

    ResurrectResult();

    explicit ResurrectResult(const ResurrectStatus& status);

    ResurrectResult(const ResurrectStatus& status, const AllyType& ally);
};


#endif  // RESURRECT_RESULT_H
