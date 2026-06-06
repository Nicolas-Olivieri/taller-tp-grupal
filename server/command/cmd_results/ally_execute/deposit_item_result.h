#ifndef DEPOSIT_ITEM_RESULT_H
#define DEPOSIT_ITEM_RESULT_H

#include "common/npc_type.h"


enum class DepositItemStatus {
    ITEM_DEPOSITED,
    ITEM_NOT_OWNED,
    ITEM_EQUIPPED,
    BANK_FULL,
    ACTION_NOT_ACCEPTED,
    PLAYER_UNBOUNDED,
    NO_RESULT,
};


struct DepositItemResult {
    DepositItemStatus status{DepositItemStatus::NO_RESULT};
    AllyType ally{AllyType::NO_ALLY};
};


#endif  // DEPOSIT_ITEM_RESULT_H
