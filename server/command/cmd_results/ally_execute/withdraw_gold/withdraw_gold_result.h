#ifndef WITHDRAW_GOLD_RESULT_H
#define WITHDRAW_GOLD_RESULT_H

#include "common/npc_type.h"


enum class WithdrawGoldStatus {
    GOLD_WITHDRAWN,
    NOT_ENOUGH_GOLD,
    INVENTORY_FULL,
    GHOST_FAIL,
    ACTION_NOT_ACCEPTED,
    PLAYER_UNBOUNDED,
    NO_RESULT,
};


struct WithdrawGoldResult {
    WithdrawGoldStatus status;
    AllyType ally;

    WithdrawGoldResult();

    explicit WithdrawGoldResult(const WithdrawGoldStatus& status);

    WithdrawGoldResult(const WithdrawGoldStatus& status, const AllyType& ally);
};


#endif  // WITHDRAW_GOLD_RESULT_H
