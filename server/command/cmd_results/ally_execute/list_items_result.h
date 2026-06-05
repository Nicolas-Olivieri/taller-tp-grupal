#ifndef LIST_ITEMS_RESULT_H
#define LIST_ITEMS_RESULT_H

#include <cstdint>
#include <map>

#include "common/npc_type.h"


struct ListItemsResult {
    bool was_player_bounded{false};
    AllyType ally{AllyType::NO_ALLY};
    std::map<uint8_t, uint16_t> items{};
};


#endif  // LIST_ITEMS_RESULT_H
