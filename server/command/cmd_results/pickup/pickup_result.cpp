#include "pickup_result.h"

PickUpResult::PickUpResult(): status(PickUpStatus::MUST_NOT_NOTIFY) {}

PickUpResult::PickUpResult(PickUpStatus status): status(status) {}
