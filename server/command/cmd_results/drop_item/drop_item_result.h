#ifndef DROP_ITEM_RESULT_H
#define DROP_ITEM_RESULT_H


enum class DropItemStatus { SUCCESS, ITEM_NOT_OWNED, ITEM_EQUIPPED, MUST_NOT_NOTIFY };


struct DropItemResult {
    DropItemStatus status;

    DropItemResult();

    explicit DropItemResult(const DropItemStatus& status);
};


#endif  // DROP_ITEM_RESULT_H
