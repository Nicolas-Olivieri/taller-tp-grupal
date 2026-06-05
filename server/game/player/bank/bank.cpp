#include "bank.h"

#include <cassert>
#include <iostream>

#include "server/persistance/playerdata.h"


Bank::Bank(const uint16_t gold_amount, const std::span<const uint8_t> items_id,
           const std::span<const uint8_t> items_amount):
        current_gold(gold_amount) {
    assert(items_id.size() == items_amount.size());
    assert(items_id.size() == BANK_AMOUNT);

    for (size_t i = 0; i < items_id.size(); ++i) {
        if (items_id[i] != 0 and items_amount[i] != 0) {
            items[items_id[i]] = items_amount[i];
        }
    }
}


uint16_t Bank::get_gold() const { return current_gold; }


void Bank::deposit_gold(const uint16_t amount) {
    if (static_cast<uint32_t>(current_gold) + amount > UINT16_MAX) {
        throw BankFull();
    }

    current_gold += amount;
}


void Bank::withdraw_gold(const uint16_t amount) {
    if (amount > current_gold) {
        throw NotEnoughGoldInBank();
    }

    current_gold -= amount;
}


const std::map<uint8_t, uint8_t>& Bank::get_items() const { return items; }


void Bank::deposit_item(const uint8_t item_id) {
    if (not items.contains(item_id)) {
        deposit_new_item(item_id);
        return;
    }

    items[item_id]++;
}


void Bank::withdraw_item(const uint8_t item_id) {
    if (not items.contains(item_id)) {
        throw ItemNotInBank();
    }

    items[item_id]--;
    if (items[item_id] == 0) {
        items.erase(item_id);
    }
}


void Bank::deposit_new_item(const uint8_t item_id) {
    if (items.size() >= BANK_AMOUNT) {
        throw BankFull();
    }

    items[item_id] = 1;
}
