#ifndef SELL_RESULT_H
#define SELL_RESULT_H

#include "common/npc_type.h"


enum class SellStatus {
    ITEM_BOUGHT,
    ITEM_NOT_OWNED,
    ITEM_EQUIPPED,
    GHOST_FAIL,
    ACTION_NOT_ACCEPTED,
    PLAYER_UNBOUNDED,
    NO_RESULT,
};


struct SellResult {
    SellStatus status;
    AllyType ally;

    SellResult();

    explicit SellResult(const SellStatus& status);

    SellResult(const SellStatus& status, const AllyType& ally);
};


#endif  // SELL_RESULT_H
