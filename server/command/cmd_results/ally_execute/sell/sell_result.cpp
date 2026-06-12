#include "sell_result.h"


SellResult::SellResult(): status(SellStatus::NO_RESULT), ally(AllyType::NO_ALLY) {}


SellResult::SellResult(const SellStatus& status): status(status), ally(AllyType::NO_ALLY) {}


SellResult::SellResult(const SellStatus& status, const AllyType& ally): status(status), ally(ally) {}
