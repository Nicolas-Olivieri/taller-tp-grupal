#include "ally_execute_result.h"


// Interacción sin resultado
AllyExecuteResult::AllyExecuteResult(): heal(HealResult::NO_RESULT), resurrect(ResurrectResult::NO_RESULT) {}


AllyExecuteResult::AllyExecuteResult(const ResurrectResult& result):
        heal(HealResult::NO_RESULT), resurrect(result) {}


AllyExecuteResult::AllyExecuteResult(const HealResult& result):
        heal(result), resurrect(ResurrectResult::NO_RESULT) {}
