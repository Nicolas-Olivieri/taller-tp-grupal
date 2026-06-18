#ifndef LIST_BANK_RESULT_H
#define LIST_BANK_RESULT_H

#include <cstdint>
#include <map>

#include "common/npc_type.h"


struct ListBankResult {
    bool was_player_bounded;
    AllyType ally;
    uint16_t gold;
    std::map<uint8_t, uint16_t> items;

    ListBankResult();

    explicit ListBankResult(bool was_bounded);

    ListBankResult(bool was_player_bounded, const AllyType& ally, uint16_t gold,
                   const std::map<uint8_t, uint16_t>& items);
};


#endif  // LIST_BANK_RESULT_H
