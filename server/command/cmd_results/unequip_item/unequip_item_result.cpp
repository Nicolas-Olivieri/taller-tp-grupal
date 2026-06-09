#include "unequip_item_result.h"


UnequipItemResult::UnequipItemResult(): status(UnequipItemStatus::MUST_NOT_NOTIFY) {}


UnequipItemResult::UnequipItemResult(const UnequipItemStatus& status): status(status) {}
