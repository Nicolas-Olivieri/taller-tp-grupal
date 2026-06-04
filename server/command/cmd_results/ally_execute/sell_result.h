#ifndef SELL_RESULT_H
#define SELL_RESULT_H


enum class SellStatus {
    ITEM_BOUGHT,
    ITEM_NOT_OWNED,
    ITEM_EQUIPPED,
    ITEM_NOT_ACCEPTED,
    PLAYER_UNBOUNDED,
    NO_RESULT,
};


struct SellResult {
    SellStatus status{SellStatus::NO_RESULT};
    AllyType ally{AllyType::NO_ALLY};
};


#endif  // SELL_RESULT_H
