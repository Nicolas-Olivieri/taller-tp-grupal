#include "ally_execute_result.h"


// Interacción sin resultado
AllyExecuteResult::AllyExecuteResult(const bool was_bounded):
        buy(was_bounded ? BuyStatus::NO_RESULT : BuyStatus::PLAYER_UNBOUNDED),
        heal(was_bounded ? HealStatus::NO_RESULT : HealStatus::PLAYER_UNBOUNDED),
        list_items(was_bounded),
        resurrect(was_bounded ? ResurrectStatus::NO_RESULT : ResurrectStatus::PLAYER_UNBOUNDED),
        sell(was_bounded ? SellStatus::NO_RESULT : SellStatus::PLAYER_UNBOUNDED) {}


AllyExecuteResult::AllyExecuteResult(const BuyResult& result): buy(result) {}


AllyExecuteResult::AllyExecuteResult(const HealResult& result): heal(result) {}


AllyExecuteResult::AllyExecuteResult(const ListItemsResult& result): list_items(result) {}


AllyExecuteResult::AllyExecuteResult(const ResurrectResult& result): resurrect(result) {}


AllyExecuteResult::AllyExecuteResult(const SellResult& result): sell(result) {}
