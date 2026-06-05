#ifndef BANKER_H
#define BANKER_H

#include "ally.h"


class Banker: public Ally {
public:
    explicit Banker(const Position& position);

    AllyExecuteResult execute(Player& player, const AllyActionPayload& payload) const override;

    ~Banker() override = default;

private:
    AllyExecuteResult handle_deposit_gold(Player& player, uint16_t gold_amount) const;

    AllyExecuteResult handle_deposit_item(Player& player, uint8_t item_id) const;

    AllyExecuteResult handle_list_items(const Player& player) const;

    AllyExecuteResult handle_withdraw_gold(Player& player, uint16_t gold_amount) const;

    AllyExecuteResult handle_withdraw_item(Player& player, uint8_t item_id) const;
};


#endif  // BANKER_H
