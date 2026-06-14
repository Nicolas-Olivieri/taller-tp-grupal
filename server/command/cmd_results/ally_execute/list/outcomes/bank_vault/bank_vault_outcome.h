#ifndef BANK_VAULT_OUTCOME_H
#define BANK_VAULT_OUTCOME_H

#include <cstdint>
#include <map>

#include "server/command/cmd_results/ally_execute/list/outcomes/list_outcomes.h"


struct BankVaultOutcome: public ListOutcome {
    uint16_t gold;
    std::map<uint8_t, uint16_t> items;

    BankVaultOutcome();

    BankVaultOutcome(uint16_t gold, const std::map<uint8_t, uint16_t>& items);
};


#endif  // BANK_VAULT_OUTCOME_H
