#include "ally_execute_result.h"


// Interacción sin resultado
AllyExecuteResult::AllyExecuteResult(const bool was_bounded):
        buy(was_bounded ? BuyStatus::NO_RESULT : BuyStatus::PLAYER_UNBOUNDED),
        heal(was_bounded ? HealResult::NO_RESULT : HealResult::PLAYER_UNBOUNDED),
        list_items(),
        resurrect(was_bounded ? ResurrectResult::NO_RESULT : ResurrectResult::PLAYER_UNBOUNDED) {}


AllyExecuteResult::AllyExecuteResult(const BuyResult& result):
        buy(result), heal(HealResult::NO_RESULT), list_items(), resurrect(ResurrectResult::NO_RESULT) {}


AllyExecuteResult::AllyExecuteResult(const HealResult& result):
        buy(), heal(result), list_items(), resurrect(ResurrectResult::NO_RESULT) {}


AllyExecuteResult::AllyExecuteResult(const ListItemsResult& result):
        buy(), heal(HealResult::NO_RESULT), list_items(result), resurrect(ResurrectResult::NO_RESULT) {}


AllyExecuteResult::AllyExecuteResult(const ResurrectResult& result):
        buy(), heal(HealResult::NO_RESULT), list_items(), resurrect(result) {}
