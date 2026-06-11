#include "interact_result.h"

#include <map>
#include <stdexcept>

#include "common/npc_type.h"

InteractResult::InteractResult(): type(InteractionType::MUST_NOT_NOTIFY), attack(), bind() {
    attack.weapon = 0;
    attack.damage_dealt = 0;
    attack.was_killed = false;
    attack.player_attacked = "";
}

InteractResult::InteractResult(const AttackStatus attackStatus): type(InteractionType::ATTACK), bind() {
    attack.status = attackStatus;
    attack.weapon = 0;
    attack.damage_dealt = 0;
    attack.was_killed = false;
    attack.player_attacked = "";
}

InteractResult::InteractResult(const uint8_t weapon, const int damage_dealt, const bool was_killed):
        type(InteractionType::ATTACK), bind() {
    attack.status = AttackStatus::HIT;
    attack.weapon = weapon;
    attack.damage_dealt = damage_dealt;
    attack.was_killed = was_killed;
    attack.player_attacked = "";
}

InteractResult::InteractResult(const AllyType& allyType): type(InteractionType::BIND), attack() {
    std::map<AllyType, BindResult> result_map({
            {AllyType::PRIEST, BindResult::PRIEST},
            {AllyType::MERCHANT, BindResult::MERCHANT},
            {AllyType::BANKER, BindResult::BANKER},
    });

    if (!result_map.contains(allyType))
        throw std::runtime_error(
                "Interact Result encontró un tipo de Aliado desconocido al procesar un bind");

    bind = result_map.at(allyType);
}
