#ifndef WITHDRAW_ITEM_RESULT_H
#define WITHDRAW_ITEM_RESULT_H

#include "common/npc_type.h"


enum class WithdrawItemStatus {
    ITEM_WITHDRAWN,
    ITEM_NOT_IN_BANK,
    INVENTORY_FULL,
    GHOST_FAIL,
    ACTION_NOT_ACCEPTED,
    PLAYER_UNBOUNDED,
    NO_RESULT,
};


struct WithdrawItemResult {
    WithdrawItemStatus status;
    AllyType ally;

    WithdrawItemResult();

    explicit WithdrawItemResult(const WithdrawItemStatus& status);

    WithdrawItemResult(const WithdrawItemStatus& status, const AllyType& ally);
};


#endif  // WITHDRAW_ITEM_RESULT_H
