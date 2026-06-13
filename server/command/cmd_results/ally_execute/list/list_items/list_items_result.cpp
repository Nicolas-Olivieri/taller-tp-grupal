#include "list_items_result.h"


ListItemsResult::ListItemsResult(): was_player_bounded(false), ally(AllyType::NO_ALLY) {}


ListItemsResult::ListItemsResult(const bool was_bounded):
        was_player_bounded(was_bounded), ally(AllyType::NO_ALLY) {}


ListItemsResult::ListItemsResult(const bool was_player_bounded, const AllyType& ally,
                                 const std::map<uint8_t, uint16_t>& items):
        was_player_bounded(was_player_bounded), ally(ally), items(items) {}
