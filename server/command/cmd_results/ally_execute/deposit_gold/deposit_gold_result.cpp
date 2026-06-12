#include "deposit_gold_result.h"


DepositGoldResult::DepositGoldResult(): status(DepositGoldStatus::NO_RESULT), ally(AllyType::NO_ALLY) {}


DepositGoldResult::DepositGoldResult(const DepositGoldStatus& status):
        status(status), ally(AllyType::NO_ALLY) {}


DepositGoldResult::DepositGoldResult(const DepositGoldStatus& status, const AllyType& ally):
        status(status), ally(ally) {}
