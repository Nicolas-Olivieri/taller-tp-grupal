#ifndef DEPOSIT_ITEM_RESULT_H
#define DEPOSIT_ITEM_RESULT_H

#include "common/npc_type.h"


enum class DepositItemStatus {
    ITEM_DEPOSITED,
    ITEM_NOT_OWNED,
    ITEM_EQUIPPED,
    BANK_FULL,
    GHOST_FAIL,
    ACTION_NOT_ACCEPTED,
    PLAYER_UNBOUNDED,
    NO_RESULT,
};


struct DepositItemResult {
    DepositItemStatus status;
    AllyType ally;

    DepositItemResult();

    explicit DepositItemResult(const DepositItemStatus& status);

    DepositItemResult(const DepositItemStatus& status, const AllyType& ally);
};


#endif  // DEPOSIT_ITEM_RESULT_H
