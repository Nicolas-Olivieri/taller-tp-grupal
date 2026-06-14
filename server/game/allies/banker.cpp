#include "banker.h"

#include <map>

#include "server/game/player/player.h"


Banker::Banker(const Position& position): Ally(position, AllyType::BANKER) {}


AllyExecuteResult Banker::execute(Player& player, const AllyActionPayload& payload) const {
    switch (payload.action) {
        case AllyAction::BUY:
            return handle_action_not_accepted<BuyResult>(BuyStatus::ACTION_NOT_ACCEPTED);

        case AllyAction::DEPOSIT_GOLD:
            return handle_deposit_gold(player, payload.gold_amount);

        case AllyAction::DEPOSIT_ITEM:
            return handle_deposit_item(player, payload.item_id);

        case AllyAction::HEAL:
            return handle_action_not_accepted<HealResult>(HealStatus::ACTION_NOT_ACCEPTED);

        case AllyAction::LIST_ITEMS:
            return handle_list_items(player);

        case AllyAction::RESURRECT:
            return handle_action_not_accepted<ResurrectResult>(ResurrectStatus::ACTION_NOT_ACCEPTED);

        case AllyAction::SELL:
            return handle_action_not_accepted<SellResult>(SellStatus::ACTION_NOT_ACCEPTED);

        case AllyAction::WITHDRAW_GOLD:
            return handle_withdraw_gold(player, payload.gold_amount);

        case AllyAction::WITHDRAW_ITEM:
            return handle_withdraw_item(player, payload.item_id);

        default:
            break;
    }

    return AllyExecuteResult(true);
}


AllyExecuteResult Banker::handle_deposit_gold(Player& player, const uint16_t gold_amount) const {
    if (not player.is_alive())
        return AllyExecuteResult(DepositGoldResult(DepositGoldStatus::GHOST_FAIL, type));

    try {
        player.deposit_gold_to_bank(gold_amount);
        std::cout << "[Banker] Se han depositado " << static_cast<int>(gold_amount) << " monedas de oro"
                  << std::endl;
        return AllyExecuteResult(DepositGoldResult(DepositGoldStatus::GOLD_DEPOSITED, type));

    } catch (const NotEnoughGold&) {
        return AllyExecuteResult(DepositGoldResult(DepositGoldStatus::NOT_ENOUGH_GOLD, type));

    } catch (const BankFull&) {
        return AllyExecuteResult(DepositGoldResult(DepositGoldStatus::BANK_FULL, type));
    }
}


AllyExecuteResult Banker::handle_deposit_item(Player& player, const uint8_t item_id) const {
    if (not player.is_alive())
        return AllyExecuteResult(DepositItemResult(DepositItemStatus::GHOST_FAIL, type));

    try {
        player.deposit_item_to_bank(item_id);
        std::cout << "[Banker] Se ha depositado el ítem de ID " << static_cast<int>(item_id) << std::endl;
        return AllyExecuteResult(DepositItemResult(DepositItemStatus::ITEM_DEPOSITED, type));

    } catch (const ItemNotOwned&) {
        return AllyExecuteResult(DepositItemResult(DepositItemStatus::ITEM_NOT_OWNED, type));

    } catch (const ItemEquipped&) {
        return AllyExecuteResult(DepositItemResult(DepositItemStatus::ITEM_EQUIPPED, type));

    } catch (const BankFull&) {
        return AllyExecuteResult(DepositItemResult(DepositItemStatus::BANK_FULL, type));
    }
}


AllyExecuteResult Banker::handle_list_items(const Player& player) const {
    const uint16_t gold_amount = player.get_bank_gold();
    std::map<uint8_t, uint16_t> items;
    for (const auto& [item_id, amount]: player.get_bank_items()) {
        items[item_id] = amount;
    }

    return AllyExecuteResult(ListBankResult(true, type, gold_amount, items));
}


AllyExecuteResult Banker::handle_withdraw_gold(Player& player, const uint16_t gold_amount) const {
    if (not player.is_alive())
        return AllyExecuteResult(WithdrawGoldResult(WithdrawGoldStatus::GHOST_FAIL, type));

    try {
        player.withdraw_gold_from_bank(gold_amount);
        std::cout << "[Banker] Se han retirado " << static_cast<int>(gold_amount) << " monedas de oro"
                  << std::endl;
        return AllyExecuteResult(WithdrawGoldResult(WithdrawGoldStatus::GOLD_WITHDRAWN, type));

    } catch (const NotEnoughGoldInBank&) {
        return AllyExecuteResult(WithdrawGoldResult(WithdrawGoldStatus::NOT_ENOUGH_GOLD, type));

    } catch (const InventoryFull&) {
        return AllyExecuteResult(WithdrawGoldResult(WithdrawGoldStatus::INVENTORY_FULL, type));
    }
}


AllyExecuteResult Banker::handle_withdraw_item(Player& player, const uint8_t item_id) const {
    if (not player.is_alive())
        return AllyExecuteResult(WithdrawItemResult(WithdrawItemStatus::GHOST_FAIL, type));

    try {
        player.withdraw_item_from_bank(item_id);
        std::cout << "[Banker] Se ha retirado el ítem de ID " << static_cast<int>(item_id) << std::endl;
        return AllyExecuteResult(WithdrawItemResult(WithdrawItemStatus::ITEM_WITHDRAWN, type));

    } catch (const ItemNotInBank&) {
        return AllyExecuteResult(WithdrawItemResult(WithdrawItemStatus::ITEM_NOT_IN_BANK, type));

    } catch (const InventoryFull&) {
        return AllyExecuteResult(WithdrawItemResult(WithdrawItemStatus::INVENTORY_FULL, type));
    }
}
