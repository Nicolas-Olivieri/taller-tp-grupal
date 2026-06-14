#include "meditate_result.h"


MeditateResult::MeditateResult(): status(MeditateStatus::MUST_NOT_NOTIFY) {}


MeditateResult::MeditateResult(const MeditateStatus& status): status(status) {}
