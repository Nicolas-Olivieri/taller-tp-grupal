#include "list_bank_result.h"


ListBankResult::ListBankResult(): was_player_bounded(false), ally(AllyType::NO_ALLY), gold(0) {}


ListBankResult::ListBankResult(const bool was_bounded):
        was_player_bounded(was_bounded), ally(AllyType::NO_ALLY), gold(0) {}


ListBankResult::ListBankResult(const bool was_player_bounded, const AllyType& ally, const uint16_t gold,
                               const std::map<uint8_t, uint16_t>& items):
        was_player_bounded(was_player_bounded), ally(ally), gold(gold), items(items) {}
