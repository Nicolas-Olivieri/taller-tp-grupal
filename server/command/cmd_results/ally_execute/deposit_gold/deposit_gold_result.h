#ifndef DEPOSIT_GOLD_RESULT_H
#define DEPOSIT_GOLD_RESULT_H

#include "common/npc_type.h"


enum class DepositGoldStatus {
    GOLD_DEPOSITED,
    NOT_ENOUGH_GOLD,
    BANK_FULL,
    GHOST_FAIL,
    ACTION_NOT_ACCEPTED,
    PLAYER_UNBOUNDED,
    NO_RESULT,
};


struct DepositGoldResult {
    DepositGoldStatus status;
    AllyType ally;

    DepositGoldResult();

    explicit DepositGoldResult(const DepositGoldStatus& status);

    DepositGoldResult(const DepositGoldStatus& status, const AllyType& ally);
};


#endif  // DEPOSIT_GOLD_RESULT_H
