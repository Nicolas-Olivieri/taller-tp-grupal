#ifndef INTERACT_RESULT_H
#define INTERACT_RESULT_H

#include "attack_result.h"

enum class InteractionType { ATTACK, BIND, MUST_NOT_NOTIFY };

struct InteractResult {
    InteractionType type;

    AttackResult attack;


    // TODO para bind podríamos pasar las coordenadas para q brille el ally
    //  El constructor default equivaldría a MUST NOT NOTIFY (actual false)
    //  Y el constructor que recibe una Position o un (x,y) es de BIND (actual)
    explicit InteractResult(const bool is_bind);

    explicit InteractResult(const AttackStatus attackStatus);

    InteractResult(const int damage_dealt, const bool was_killed);
};


#endif  // INTERACT_RESULT_H
