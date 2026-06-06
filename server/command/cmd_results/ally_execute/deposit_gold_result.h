#ifndef DEPOSIT_GOLD_RESULT_H
#define DEPOSIT_GOLD_RESULT_H

#include "common/npc_type.h"


enum class DepositGoldStatus {
    GOLD_DEPOSITED,
    NOT_ENOUGH_GOLD,
    BANK_FULL,
    ACTION_NOT_ACCEPTED,
    PLAYER_UNBOUNDED,
    NO_RESULT,
};


struct DepositGoldResult {
    DepositGoldStatus status{DepositGoldStatus::NO_RESULT};
    AllyType ally{AllyType::NO_ALLY};
};


#endif  // DEPOSIT_GOLD_RESULT_H
