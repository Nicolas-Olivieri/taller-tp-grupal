#ifndef RECOVER_RESULT_H
#define RECOVER_RESULT_H

#include <cstdint>
#include <string>

enum class RecoverStatus {
    RECOVER,
    OUT_OF_RANGE,
    DEAD_TARGET,
    CANNOT_HEAL,
    CANNOT_HEAL_CREATURE,
    COMPLETE,
};

struct RecoverResult {
    RecoverStatus status;

    std::uint8_t weapon;
    uint16_t recover_amount;
    std::string player_recovered;
};

#endif  // RECOVER_RESULT_H
