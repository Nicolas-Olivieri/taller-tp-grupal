#ifndef BUY_RESULT_H
#define BUY_RESULT_H

#include "common/npc_type.h"


enum class BuyStatus {
    ITEM_SOLD,
    ITEM_NOT_SOLD,
    NOT_ENOUGH_GOLD,
    INVENTORY_FULL,
    GHOST_FAIL,
    ACTION_NOT_ACCEPTED,
    PLAYER_UNBOUNDED,
    NO_RESULT,
};


struct BuyResult {
    BuyStatus status;
    AllyType ally;

    BuyResult();

    explicit BuyResult(const BuyStatus& status);

    BuyResult(const BuyStatus& status, const AllyType& ally);
};


#endif  // BUY_RESULT_H
