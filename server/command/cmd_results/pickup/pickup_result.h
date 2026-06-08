#ifndef PICKUP_RESULT_H
#define PICKUP_RESULT_H

#include <cstdint>

enum class PickUpStatus : uint8_t {
    SUCCESS,
    GHOST_FAIL,
    NOT_ENOUGH_SPACE,
    GOLD_OVERFLOW,
    MUST_NOT_NOTIFY,
    NO_LOOT
};

struct PickUpResult {
    PickUpStatus status;

    PickUpResult();

    explicit PickUpResult(PickUpStatus status);
};

#endif  // PICKUP_RESULT_H
