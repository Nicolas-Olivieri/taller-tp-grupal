#ifndef HEAL_RESULT_H
#define HEAL_RESULT_H

#include "common/npc_type.h"


enum class HealStatus {
    PLAYER_HEALED,
    GHOST_FAIL,
    ACTION_NOT_ACCEPTED,
    PLAYER_UNBOUNDED,
    NO_RESULT,
};


struct HealResult {
    HealStatus status;
    AllyType ally;

    HealResult();

    explicit HealResult(const HealStatus& status);

    HealResult(const HealStatus& status, const AllyType& ally);
};


#endif  // HEAL_RESULT_H
