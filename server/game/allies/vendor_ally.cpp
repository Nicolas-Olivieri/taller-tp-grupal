#include "vendor_ally.h"

#include <map>

#include "server/config/game_config.h"
#include "server/game/player/player.h"

VendorAlly::VendorAlly(const Position& position, const AllyType& type, uint8_t item_set_id):
        Ally(position, type), items(assign_items(type, item_set_id)) {}

const std::vector<uint8_t>& VendorAlly::assign_items(const AllyType& type, uint8_t item_set_id) {
    switch (type) {
        case AllyType::PRIEST:
            return GameConfig::get().get_priest_items(item_set_id);
        case AllyType::MERCHANT:
            return GameConfig::get().get_merchant_items(item_set_id);
        default:
            throw std::invalid_argument("This type of ally cannot have a set of items to sell");
    }
}

AllyExecuteResult VendorAlly::handle_buy_item(Player& player, const uint8_t item_id) const {
    if (std::ranges::find(items, item_id) == items.end()) {
        return AllyExecuteResult(BuyResult(BuyStatus::ITEM_NOT_SOLD, type));
    }

    try {
        const uint16_t price = GameConfig::get().get_item_price(item_id);
        player.spend_gold(price);
        player.acquire_item(item_id);

        std::cout << "[VendorAlly] Venta exitosa del ítem " << static_cast<int>(item_id) << std::endl;
        return AllyExecuteResult(BuyResult(BuyStatus::ITEM_SOLD, type));

    } catch (const NotEnoughGold&) {
        return AllyExecuteResult(BuyResult(BuyStatus::NOT_ENOUGH_GOLD, type));

    } catch (const InventoryFull&) {
        return AllyExecuteResult(BuyResult(BuyStatus::INVENTORY_FULL, type));
    }
}


AllyExecuteResult VendorAlly::handle_list_items() const {
    std::map<uint8_t, uint16_t> items_prices;
    for (const uint8_t item_id: items) {
        items_prices[item_id] = GameConfig::get().get_item_price(item_id);
    }

    return AllyExecuteResult(ListItemsResult(true, type, items_prices));
}
