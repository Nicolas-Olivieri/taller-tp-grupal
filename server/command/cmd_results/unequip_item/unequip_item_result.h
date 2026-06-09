#ifndef UNEQUIP_ITEM_RESULT_H
#define UNEQUIP_ITEM_RESULT_H


enum class UnequipItemStatus { SUCCESS, FAILED, MUST_NOT_NOTIFY };


struct UnequipItemResult {
    UnequipItemStatus status;

    UnequipItemResult();

    explicit UnequipItemResult(const UnequipItemStatus& status);
};


#endif  // UNEQUIP_ITEM_RESULT_H
