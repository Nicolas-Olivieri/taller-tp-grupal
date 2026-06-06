#ifndef LIST_OUTCOMES_H
#define LIST_OUTCOMES_H

#include <cstdint>
#include <map>

#include "common/npc_type.h"


struct ListOutcome {
    AllyType ally{AllyType::NO_ALLY};
    virtual ~ListOutcome() = default;
};


struct PlayerUnboundOutcome: public ListOutcome {};


struct BankVaultOutcome: public ListOutcome {
    uint16_t gold{0};
    std::map<uint8_t, uint16_t> items{};
};


struct VendorListOutcome: public ListOutcome {
    std::map<uint8_t, uint16_t> items{};
};


#endif  // LIST_OUTCOMES_H
