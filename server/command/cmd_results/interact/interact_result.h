#ifndef INTERACT_RESULT_H
#define INTERACT_RESULT_H

#include "common/npc_type.h"

#include "attack_result.h"
#include "bind_result.h"

enum class InteractionType { ATTACK, BIND, MUST_NOT_NOTIFY };

struct InteractResult {
    InteractionType type;

    AttackResult attack;
    BindResult bind;


    InteractResult();

    explicit InteractResult(const AllyType& allyType);

    explicit InteractResult(const AttackStatus attackStatus);

    InteractResult(uint8_t weapon, const int damage_dealt, const bool was_killed);
};


#endif  // INTERACT_RESULT_H
