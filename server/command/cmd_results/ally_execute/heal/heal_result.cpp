#include "heal_result.h"


HealResult::HealResult(): status(HealStatus::NO_RESULT), ally(AllyType::NO_ALLY) {}


HealResult::HealResult(const HealStatus& status): status(status), ally(AllyType::NO_ALLY) {}


HealResult::HealResult(const HealStatus& status, const AllyType& ally): status(status), ally(ally) {}
