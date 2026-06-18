#include "resurrect_result.h"


ResurrectResult::ResurrectResult(): status(ResurrectStatus::NO_RESULT), ally(AllyType::NO_ALLY) {}


ResurrectResult::ResurrectResult(const ResurrectStatus& status): status(status), ally(AllyType::NO_ALLY) {}


ResurrectResult::ResurrectResult(const ResurrectStatus& status, const AllyType& ally):
        status(status), ally(ally) {}
