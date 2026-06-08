#ifndef PICKUP_RESULT_H
#define PICKUP_RESULT_H

#include <cstdint>

enum class PickUpStatus : uint8_t { SUCCESS, FAILED, MUST_NOT_NOTIFY };

struct PickUpResult {
    PickUpStatus status;

    PickUpResult();

    explicit PickUpResult(PickUpStatus status);
};

#endif  // PICKUP_RESULT_H
