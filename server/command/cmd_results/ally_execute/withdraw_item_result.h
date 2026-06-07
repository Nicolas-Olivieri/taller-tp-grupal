#ifndef WITHDRAW_ITEM_RESULT_H
#define WITHDRAW_ITEM_RESULT_H

#include "common/npc_type.h"


enum class WithdrawItemStatus {
    ITEM_WITHDRAWN,
    ITEM_NOT_IN_BANK,
    INVENTORY_FULL,
    ACTION_NOT_ACCEPTED,
    PLAYER_UNBOUNDED,
    NO_RESULT,
};


struct WithdrawItemResult {
    WithdrawItemStatus status{WithdrawItemStatus::NO_RESULT};
    AllyType ally{AllyType::NO_ALLY};
};


#endif  // WITHDRAW_ITEM_RESULT_H
