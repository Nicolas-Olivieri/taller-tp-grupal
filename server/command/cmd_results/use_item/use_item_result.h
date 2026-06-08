#ifndef USE_ITEM_RESULT_H
#define USE_ITEM_RESULT_H


enum class UseItemStatus { SUCCESS, FAILED, MUST_NOT_NOTIFY };


struct UseItemResult {
    UseItemStatus status;

    UseItemResult();

    explicit UseItemResult(const UseItemStatus& status);
};


#endif  // USE_ITEM_RESULT_H
