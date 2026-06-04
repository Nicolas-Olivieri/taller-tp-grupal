#include "ally_execute_result.h"


// Interacción sin resultado
AllyExecuteResult::AllyExecuteResult(const bool was_bounded):
        heal(was_bounded ? HealResult::NO_RESULT : HealResult::PLAYER_UNBOUNDED),
        list_items(),
        resurrect(was_bounded ? ResurrectResult::NO_RESULT : ResurrectResult::PLAYER_UNBOUNDED) {}


AllyExecuteResult::AllyExecuteResult(const HealResult& result):
        heal(result), list_items(), resurrect(ResurrectResult::NO_RESULT) {}


AllyExecuteResult::AllyExecuteResult(const ListItemsResult& result):
        heal(HealResult::NO_RESULT), list_items(result), resurrect(ResurrectResult::NO_RESULT) {}


AllyExecuteResult::AllyExecuteResult(const ResurrectResult& result):
        heal(HealResult::NO_RESULT), list_items(), resurrect(result) {}
