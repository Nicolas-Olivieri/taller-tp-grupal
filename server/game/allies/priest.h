#ifndef PRIEST_H
#define PRIEST_H


#include "vendor_ally.h"


class Priest: public VendorAlly {
public:
    explicit Priest(const Position& position);

    AllyExecuteResult execute(Player& player, const AllyActionPayload& payload) const override;

    ~Priest() override = default;

private:
    AllyExecuteResult handle_heal(Player& player) const;

    AllyExecuteResult handle_resurrect(Player& player) const;
};


#endif  // PRIEST_H
