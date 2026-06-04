#ifndef HEAL_RESULT_H
#define HEAL_RESULT_H


enum class HealStatus {
    PLAYER_HEALED,
    PLAYER_IS_DEAD,
    ACTION_NOT_ACCEPTED,
    PLAYER_UNBOUNDED,
    NO_RESULT,
};


struct HealResult {
    HealStatus status{HealStatus::NO_RESULT};
    AllyType ally{AllyType::NO_ALLY};
};


#endif  // HEAL_RESULT_H
