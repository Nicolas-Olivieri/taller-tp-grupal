#ifndef MERCHANT_H
#define MERCHANT_H

#include "vendor_ally.h"


class Merchant: public VendorAlly {
public:
    explicit Merchant(const Position& position);

    AllyExecuteResult execute(Player& player, const AllyActionPayload& payload) const override;

    ~Merchant() override = default;

private:
    AllyExecuteResult handle_sell_item(Player& player, uint8_t item_id) const;

    AllyExecuteResult handle_heal() const;

    AllyExecuteResult handle_resurrect() const;
};


#endif  // MERCHANT_H
