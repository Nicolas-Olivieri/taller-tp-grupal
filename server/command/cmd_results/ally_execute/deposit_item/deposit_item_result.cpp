#include "deposit_item_result.h"


DepositItemResult::DepositItemResult(): status(DepositItemStatus::NO_RESULT), ally(AllyType::NO_ALLY) {}


DepositItemResult::DepositItemResult(const DepositItemStatus& status):
        status(status), ally(AllyType::NO_ALLY) {}


DepositItemResult::DepositItemResult(const DepositItemStatus& status, const AllyType& ally):
        status(status), ally(ally) {}
