#ifndef PRIEST_H
#define PRIEST_H

#include "ally.h"


class Priest: public Ally {
public:
    explicit Priest(const Position& position);

    AllyExecuteResult execute(Player& player, const AllyActionPayload& payload) override;

    AllyType get_type() const override;

    ~Priest() override = default;

private:
    AllyExecuteResult handle_heal(Player& player);

    AllyExecuteResult handle_resurrect(Player& player);
};


#endif  // PRIEST_H
