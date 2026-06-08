#include "pickup_result.h"

PickUpResult::PickUpResult(): status(PickUpStatus::MUST_NOT_NOTIFY), picked_something_up(false) {}

PickUpResult::PickUpResult(PickUpStatus status): status(status) {
    picked_something_up = status == PickUpStatus::SUCCESS || status == PickUpStatus::PARTIAL;
}
