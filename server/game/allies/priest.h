#ifndef PRIEST_H
#define PRIEST_H

#include <map>

#include "ally.h"


class Priest: public Ally {
private:
    std::map<uint8_t, uint16_t> items;  // { ID, precio }

public:
    explicit Priest(const Position& position);

    AllyExecuteResult execute(Player& player, const AllyActionPayload& payload) override;

    AllyType get_type() const override;

    ~Priest() override = default;

private:
    AllyExecuteResult handle_heal(Player& player);

    AllyExecuteResult handle_resurrect(Player& player);

    AllyExecuteResult handle_list_items();
};


#endif  // PRIEST_H
