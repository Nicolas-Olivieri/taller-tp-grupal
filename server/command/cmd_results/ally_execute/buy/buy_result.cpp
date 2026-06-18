#include "buy_result.h"


BuyResult::BuyResult(): status(BuyStatus::NO_RESULT), ally(AllyType::NO_ALLY) {}


BuyResult::BuyResult(const BuyStatus& status): status(status), ally(AllyType::NO_ALLY) {}


BuyResult::BuyResult(const BuyStatus& status, const AllyType& ally): status(status), ally(ally) {}
