#include "merchant.h"

#include "server/config/game_config.h"
#include "server/game/player/player.h"
#include "server/util/calculator.h"

Merchant::Merchant(const Position& position):
        VendorAlly(position, AllyType::MERCHANT,
                   Calculator::random_number(0, GameConfig::get().get_merchant_max_id())) {}


AllyExecuteResult Merchant::execute(Player& player, const AllyActionPayload& payload) const {
    switch (payload.action) {
        case AllyAction::BUY:
            return handle_buy_item(player, payload.item_id);

        case AllyAction::DEPOSIT_GOLD:
            return handle_action_not_accepted<DepositGoldResult>(DepositGoldStatus::ACTION_NOT_ACCEPTED);

        case AllyAction::DEPOSIT_ITEM:
            return handle_action_not_accepted<DepositItemResult>(DepositItemStatus::ACTION_NOT_ACCEPTED);

        case AllyAction::HEAL:
            return handle_action_not_accepted<HealResult>(HealStatus::ACTION_NOT_ACCEPTED);

        case AllyAction::LIST_ITEMS:
            return handle_list_items();

        case AllyAction::RESURRECT:
            return handle_action_not_accepted<ResurrectResult>(ResurrectStatus::ACTION_NOT_ACCEPTED);

        case AllyAction::SELL:
            return handle_sell_item(player, payload.item_id);

        case AllyAction::WITHDRAW_GOLD:
            return handle_action_not_accepted<WithdrawGoldResult>(WithdrawGoldStatus::ACTION_NOT_ACCEPTED);

        case AllyAction::WITHDRAW_ITEM:
            return handle_action_not_accepted<WithdrawItemResult>(WithdrawItemStatus::ACTION_NOT_ACCEPTED);

        default:
            break;
    }

    return AllyExecuteResult(true);
}


AllyExecuteResult Merchant::handle_sell_item(Player& player, const uint8_t item_id) const {
    try {
        const uint16_t price = GameConfig::get().get_item_price(item_id);
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
