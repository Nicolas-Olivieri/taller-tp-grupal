#ifndef ALLY_ACTION_PAYLOAD_H
#define ALLY_ACTION_PAYLOAD_H

#include <cstdint>


enum class AllyAction {
    BUY,
    DEPOSIT_GOLD,
    DEPOSIT_ITEM,
    HEAL,
    LIST_ITEMS,
    RESURRECT,
    SELL,
    WITHDRAW_GOLD,
    WITHDRAW_ITEM,
};


struct AllyActionPayload {
    AllyAction action;
    uint8_t item_id;       // ID del ítem a vender o comprar
    uint16_t gold_amount;  // Cantidad de oro a depositar o retirar

    explicit AllyActionPayload(const AllyAction& action);

    AllyActionPayload(const AllyAction& action, uint8_t item_id);

    AllyActionPayload(const AllyAction& action, uint16_t gold_amount);
};


#endif  // ALLY_ACTION_PAYLOAD_H
