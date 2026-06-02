#include "interact_result.h"

InteractResult::InteractResult(const bool is_bind): attack() {
    type = is_bind ? InteractionType::BIND : InteractionType::MUST_NOT_NOTIFY;
}

InteractResult::InteractResult(const AttackStatus attackStatus): type(InteractionType::ATTACK) {
    attack.status = attackStatus;
}

InteractResult::InteractResult(const int damage_dealt, const bool was_killed): type(InteractionType::ATTACK) {
    attack.status = AttackStatus::HIT;
    attack.damage_dealt = damage_dealt;
    attack.was_killed = was_killed;
}
