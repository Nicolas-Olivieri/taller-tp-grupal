#ifndef PRIEST_H
#define PRIEST_H

#include <vector>

#include "ally.h"


class Priest: public Ally {
private:
    std::vector<uint8_t> items;  // vector de ID de ítems

public:
    explicit Priest(const Position& position);

    AllyExecuteResult execute(Player& player, const AllyActionPayload& payload) override;

    AllyType get_type() const override;

    ~Priest() override = default;

private:
    AllyExecuteResult handle_heal(Player& player);

    AllyExecuteResult handle_resurrect(Player& player);

    AllyExecuteResult handle_list_items();

    AllyExecuteResult handle_buy_item(Player& player, uint8_t item_id);
};


#endif  // PRIEST_H
