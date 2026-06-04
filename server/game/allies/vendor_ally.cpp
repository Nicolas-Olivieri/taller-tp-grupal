#include "vendor_ally.h"

#include <map>

#include "server/game/player/inventory/item_mapper.h"
#include "server/game/player/player.h"


VendorAlly::VendorAlly(const Position& position, const AllyType& type, const std::vector<uint8_t>& items):
        Ally(position, type), items(items) {}


AllyExecuteResult VendorAlly::handle_buy_item(Player& player, const uint8_t item_id) const {
    if (std::ranges::find(items, item_id) == items.end()) {
        return AllyExecuteResult(BuyResult(BuyStatus::ITEM_NOT_SOLD, type));
    }

    try {
        const uint16_t price = ItemMapper::get_price(item_id);
        player.spend_gold(price);
        player.acquire_item(item_id);

        std::cout << "[VendorAlly] Venta exitosa del ítem " << static_cast<int>(item_id) << std::endl;
        return AllyExecuteResult(BuyResult(BuyStatus::ITEM_SOLD, type));

    } catch (const NotEnoughGold&) {
        return AllyExecuteResult(BuyResult(BuyStatus::NOT_ENOUGH_GOLD, type));
    }
}


AllyExecuteResult VendorAlly::handle_list_items() const {
    std::map<uint8_t, uint16_t> items_prices;
    for (const uint8_t item_id: items) {
        items_prices[item_id] = ItemMapper::get_price(item_id);
    }

    return AllyExecuteResult(ListItemsResult(true, type, items_prices));
}
