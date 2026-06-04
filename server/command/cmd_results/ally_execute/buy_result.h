#ifndef BUY_RESULT_H
#define BUY_RESULT_H

#include "common/npc_type.h"


enum class BuyStatus {
    ITEM_SOLD,
    ITEM_NOT_SOLD,
    NOT_ENOUGH_GOLD,
    ACTION_NOT_ACCEPTED,
    PLAYER_UNBOUNDED,
    NO_RESULT,
};


struct BuyResult {
    BuyStatus status{BuyStatus::NO_RESULT};
    AllyType ally{AllyType::NO_ALLY};
};


#endif  // BUY_RESULT_H
