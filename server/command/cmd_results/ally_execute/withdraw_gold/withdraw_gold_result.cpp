#include "withdraw_gold_result.h"


WithdrawGoldResult::WithdrawGoldResult(): status(WithdrawGoldStatus::NO_RESULT), ally(AllyType::NO_ALLY) {}


WithdrawGoldResult::WithdrawGoldResult(const WithdrawGoldStatus& status):
        status(status), ally(AllyType::NO_ALLY) {}


WithdrawGoldResult::WithdrawGoldResult(const WithdrawGoldStatus& status, const AllyType& ally):
        status(status), ally(ally) {}
