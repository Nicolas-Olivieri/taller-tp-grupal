#ifndef LIST_ITEMS_RESULT_H
#define LIST_ITEMS_RESULT_H

#include <cstdint>
#include <map>

#include "common/npc_type.h"


struct ListItemsResult {
    bool was_player_bounded;
    AllyType ally;
    std::map<uint8_t, uint16_t> items;

    ListItemsResult();

    explicit ListItemsResult(bool was_bounded);

    ListItemsResult(bool was_player_bounded, const AllyType& ally, const std::map<uint8_t, uint16_t>& items);
};


#endif  // LIST_ITEMS_RESULT_H
