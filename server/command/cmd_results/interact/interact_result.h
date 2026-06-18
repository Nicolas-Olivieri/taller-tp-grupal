#ifndef INTERACT_RESULT_H
#define INTERACT_RESULT_H

#include <string>

#include "common/npc_type.h"

#include "attack_result.h"
#include "bind_result.h"
#include "recover_result.h"

enum class InteractionType { ATTACK, BIND, RECOVER, MUST_NOT_NOTIFY };

struct InteractResult {
    InteractionType type;

    AttackResult attack;
    RecoverResult recover;
    BindResult bind;


    InteractResult();

    explicit InteractResult(const AllyType& ally_type);

    explicit InteractResult(const AttackStatus& attack_status);

    explicit InteractResult(const RecoverStatus& recover_status);

    InteractResult(uint8_t weapon, const int damage_dealt, const bool was_killed);

    InteractResult(uint8_t weapon, const int amount, const std::string& player_name);
};


#endif  // INTERACT_RESULT_H
