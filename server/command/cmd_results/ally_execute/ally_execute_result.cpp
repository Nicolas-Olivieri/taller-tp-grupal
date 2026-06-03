#include "ally_execute_result.h"


// Interacción sin resultado
AllyExecuteResult::AllyExecuteResult():
        heal(HealResult::NO_RESULT), list_items(), resurrect(ResurrectResult::NO_RESULT) {}


AllyExecuteResult::AllyExecuteResult(const HealResult& result):
        heal(result), list_items(), resurrect(ResurrectResult::NO_RESULT) {}


AllyExecuteResult::AllyExecuteResult(const ListItemsResult& result):
        heal(HealResult::NO_RESULT), list_items(result), resurrect(ResurrectResult::NO_RESULT) {}


AllyExecuteResult::AllyExecuteResult(const ResurrectResult& result):
        heal(HealResult::NO_RESULT), list_items(), resurrect(result) {}
