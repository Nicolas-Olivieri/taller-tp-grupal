#include "drop_item_result.h"


DropItemResult::DropItemResult(): status(DropItemStatus::MUST_NOT_NOTIFY) {}


DropItemResult::DropItemResult(const DropItemStatus& status): status(status) {}
