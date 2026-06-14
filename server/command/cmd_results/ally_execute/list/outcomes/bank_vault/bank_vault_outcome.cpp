#include "bank_vault_outcome.h"


BankVaultOutcome::BankVaultOutcome(): gold(0) {}


BankVaultOutcome::BankVaultOutcome(const uint16_t gold, const std::map<uint8_t, uint16_t>& items):
        gold(gold), items(items) {}
