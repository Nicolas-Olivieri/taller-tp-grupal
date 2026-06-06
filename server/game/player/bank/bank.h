#ifndef BANK_H
#define BANK_H

#include <cstdint>
#include <map>
#include <span>
#include <stdexcept>


struct BankFull: public std::runtime_error {
    BankFull(): std::runtime_error("The bank vault is full.") {}
};


struct ItemNotInBank: public std::runtime_error {
    ItemNotInBank(): std::runtime_error("The requested item is not in the bank.") {}
};


struct NotEnoughGoldInBank: public std::runtime_error {
    NotEnoughGoldInBank(): std::runtime_error("Not enough gold in the bank account.") {}
};


class Bank {
private:
    uint16_t current_gold;
    std::map<uint8_t, uint8_t> items;  // mapa de <ID ítem, cantidad guardada en el banco>

public:
    Bank(uint16_t gold_amount, std::span<const uint8_t> items_id, std::span<const uint8_t> items_amount);

    uint16_t get_gold() const;

    void deposit_gold(uint16_t amount);

    void withdraw_gold(uint16_t amount);

    const std::map<uint8_t, uint8_t>& get_items() const;

    void deposit_item(uint8_t item_id);

    void withdraw_item(uint8_t item_id);

private:
    void deposit_new_item(uint8_t item_id);
};


#endif  // BANK_H
