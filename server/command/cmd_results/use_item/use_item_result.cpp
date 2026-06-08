#include "use_item_result.h"


UseItemResult::UseItemResult(): status(UseItemStatus::MUST_NOT_NOTIFY) {}


UseItemResult::UseItemResult(const UseItemStatus& status): status(status) {}
