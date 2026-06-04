#include "merchant.h"

#include "server/game/player/inventory/item_mapper.h"
#include "server/game/player/player.h"


Merchant::Merchant(const Position& position):
        VendorAlly(position, AllyType::MERCHANT, ItemMapper::get_random_merchant_items()) {}


AllyExecuteResult Merchant::execute(Player& player, const AllyActionPayload& payload) const {
    switch (payload.action) {
        case AllyAction::LIST_ITEMS:
            return handle_list_items();

        case AllyAction::BUY:
            return handle_buy_item(player, payload.item_id);

        case AllyAction::SELL:
            return handle_sell_item(player, payload.item_id);

        default:
            break;
    }

    return AllyExecuteResult(true);
}


AllyExecuteResult Merchant::handle_sell_item(Player& player, const uint8_t item_id) const {
    try {
        const uint16_t price = ItemMapper::get_price(item_id);
        player.drop_item(item_id);
        // TODO: El porcentaje de precio de venta debería venir del TOML
        player.add_gold(price * 0.70);

        std::cout << "[Merchant] Compra exitosa del ítem " << static_cast<int>(item_id) << " por "
                  << price * 0.70 << " monedas de oro" << std::endl;
        return AllyExecuteResult(SellResult(SellStatus::ITEM_BOUGHT, type));

    } catch (const ItemNotOwned&) {
        return AllyExecuteResult(SellResult(SellStatus::ITEM_NOT_OWNED, type));

    } catch (const ItemEquipped&) {
        return AllyExecuteResult(SellResult(SellStatus::ITEM_EQUIPPED, type));
    }
}
