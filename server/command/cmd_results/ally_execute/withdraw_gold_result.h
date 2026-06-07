#ifndef WITHDRAW_GOLD_RESULT_H
#define WITHDRAW_GOLD_RESULT_H

#include "common/npc_type.h"


enum class WithdrawGoldStatus {
    GOLD_WITHDRAWN,
    NOT_ENOUGH_GOLD,
    INVENTORY_FULL,
    ACTION_NOT_ACCEPTED,
    PLAYER_UNBOUNDED,
    NO_RESULT,
};


struct WithdrawGoldResult {
    WithdrawGoldStatus status{WithdrawGoldStatus::NO_RESULT};
    AllyType ally{AllyType::NO_ALLY};
};


#endif  // WITHDRAW_GOLD_RESULT_H
