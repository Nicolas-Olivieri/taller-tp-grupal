#ifndef VENDOR_ALLY_H
#define VENDOR_ALLY_H

#include <vector>

#include "ally.h"


class VendorAlly: public Ally {
protected:
    std::vector<uint8_t> items;  // vector de ID de ítems

    VendorAlly(const Position& position, const AllyType& type, uint8_t item_set_id);

    AllyExecuteResult handle_buy_item(Player& player, uint8_t item_id) const;

    AllyExecuteResult handle_list_items() const;

    const std::vector<uint8_t>& assign_items(const AllyType& type, uint8_t item_set_id);

public:
    ~VendorAlly() override = default;
};


#endif  // VENDOR_ALLY_H
