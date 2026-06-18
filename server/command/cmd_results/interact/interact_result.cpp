#include "interact_result.h"

#include <map>
#include <stdexcept>

#include "common/npc_type.h"

InteractResult::InteractResult(): type(InteractionType::MUST_NOT_NOTIFY), attack(), bind() {
    attack.weapon = 0;
    attack.damage_dealt = 0;
    attack.was_killed = false;
    attack.player_attacked = "";
    attack.attacked_clan_name = "";

    recover.weapon = 5;
    recover.player_recovered = "";
    recover.recover_amount = 0;
}

InteractResult::InteractResult(const AttackStatus& attack_status): type(InteractionType::ATTACK), bind() {
    attack.status = attack_status;
    attack.weapon = 0;
    attack.damage_dealt = 0;
    attack.was_killed = false;
    attack.player_attacked = "";
    attack.attacked_clan_name = "";

    recover.weapon = 5;
    recover.player_recovered = "";
    recover.recover_amount = 0;
}

InteractResult::InteractResult(const RecoverStatus& recover_status): type(InteractionType::RECOVER), bind() {
    attack.weapon = 0;
    attack.damage_dealt = 0;
    attack.was_killed = false;
    attack.player_attacked = "";
    attack.attacked_clan_name = "";

    recover.status = recover_status;
    recover.weapon = 5;
    recover.player_recovered = "";
    recover.recover_amount = 0;
}


InteractResult::InteractResult(const uint8_t weapon, const int damage_dealt, const bool was_killed):
        type(InteractionType::ATTACK), bind() {
    attack.status = AttackStatus::HIT;
    attack.weapon = weapon;
    attack.damage_dealt = damage_dealt;
    attack.was_killed = was_killed;
    attack.player_attacked = "";
    attack.attacked_clan_name = "";

    recover.weapon = 5;
    recover.player_recovered = "";
    recover.recover_amount = 0;
}

InteractResult::InteractResult(uint8_t weapon, const int amount, const std::string& player_name):
        type(InteractionType::RECOVER), bind() {
    recover.status = RecoverStatus::RECOVER;
    recover.player_recovered = player_name;
    recover.weapon = weapon;
    recover.recover_amount = amount;

    attack.weapon = 0;
    attack.damage_dealt = 0;
    attack.was_killed = false;
    attack.player_attacked = "";
    attack.attacked_clan_name = "";
}

InteractResult::InteractResult(const AllyType& ally_type): type(InteractionType::BIND), attack(), recover() {
    std::map<AllyType, BindResult> result_map({
            {AllyType::PRIEST, BindResult::PRIEST},
            {AllyType::MERCHANT, BindResult::MERCHANT},
            {AllyType::BANKER, BindResult::BANKER},
            {AllyType::TOTEM, BindResult::TOTEM},
    });

    if (!result_map.contains(ally_type))
        throw std::runtime_error(
                "Interact Result encontró un tipo de Aliado desconocido al procesar un bind");

    bind = result_map.at(ally_type);
}
