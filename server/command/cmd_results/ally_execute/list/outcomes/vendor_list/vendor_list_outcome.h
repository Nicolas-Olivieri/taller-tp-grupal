#ifndef VENDOR_LIST_OUTCOME_H
#define VENDOR_LIST_OUTCOME_H

#include <cstdint>
#include <map>

#include "server/command/cmd_results/ally_execute/list/outcomes/list_outcomes.h"


struct VendorListOutcome: public ListOutcome {
    std::map<uint8_t, uint16_t> items;

    VendorListOutcome();

    explicit VendorListOutcome(const std::map<uint8_t, uint16_t>& items);
};


#endif  // VENDOR_LIST_OUTCOME_H
