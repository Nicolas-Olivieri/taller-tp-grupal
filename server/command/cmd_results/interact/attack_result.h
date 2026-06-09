#ifndef ATTACK_RESULT_H
#define ATTACK_RESULT_H

#include <cstdint>
#include <string>

enum class AttackStatus { HIT, OUT_OF_RANGE, DEAD_TARGET, CANNOT_ATTACK, TARGET_DODGED };

// TODO va a tener que incluir el clan name para notificar a los compis
struct AttackResult {
    AttackStatus status;

    std::uint8_t weapon;
    int damage_dealt;
    bool was_killed;
    std::string player_attacked;
};

#endif  // ATTACK_RESULT_H
