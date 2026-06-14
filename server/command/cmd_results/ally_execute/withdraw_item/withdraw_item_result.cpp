#include "withdraw_item_result.h"


WithdrawItemResult::WithdrawItemResult(): status(WithdrawItemStatus::NO_RESULT), ally(AllyType::NO_ALLY) {}


WithdrawItemResult::WithdrawItemResult(const WithdrawItemStatus& status):
        status(status), ally(AllyType::NO_ALLY) {}


WithdrawItemResult::WithdrawItemResult(const WithdrawItemStatus& status, const AllyType& ally):
        status(status), ally(ally) {}
