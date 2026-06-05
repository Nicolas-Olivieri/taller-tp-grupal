#include "priest.h"


#include "server/game/player/inventory/item_mapper.h"
#include "server/game/player/player.h"


Priest::Priest(const Position& position):
        VendorAlly(position, AllyType::PRIEST, ItemMapper::get_random_priest_items()) {}


AllyExecuteResult Priest::execute(Player& player, const AllyActionPayload& payload) const {
    switch (payload.action) {
        case AllyAction::BUY:
            return handle_buy_item(player, payload.item_id);

        case AllyAction::DEPOSIT_GOLD:
            return handle_action_not_accepted<DepositGoldResult>(DepositGoldStatus::ACTION_NOT_ACCEPTED);

        case AllyAction::DEPOSIT_ITEM:
            return handle_action_not_accepted<DepositItemResult>(DepositItemStatus::ACTION_NOT_ACCEPTED);

        case AllyAction::HEAL:
            return handle_heal(player);

        case AllyAction::LIST_ITEMS:
            return handle_list_items();

        case AllyAction::RESURRECT:
            return handle_resurrect(player);

        case AllyAction::SELL:
            return handle_action_not_accepted<SellResult>(SellStatus::ACTION_NOT_ACCEPTED);

        case AllyAction::WITHDRAW_GOLD:
            return handle_action_not_accepted<WithdrawGoldResult>(WithdrawGoldStatus::ACTION_NOT_ACCEPTED);

        case AllyAction::WITHDRAW_ITEM:
            return handle_action_not_accepted<WithdrawItemResult>(WithdrawItemStatus::ACTION_NOT_ACCEPTED);

        default:
            break;
    }

    return AllyExecuteResult(true);
}


AllyExecuteResult Priest::handle_heal(Player& player) const {
    if (player.is_alive()) {
        std::cout << "[Priest] El jugador fue curado" << std::endl;
        player.heal();
        return AllyExecuteResult(HealResult(HealStatus::PLAYER_HEALED, type));
    }

    std::cout << "[Priest] El jugador está muerto" << std::endl;
    return AllyExecuteResult(HealResult(HealStatus::PLAYER_IS_DEAD, type));
}


AllyExecuteResult Priest::handle_resurrect(Player& player) const {
    if (not player.is_alive()) {
        std::cout << "[Priest] El jugador fue resucitado" << std::endl;
        player.heal();
        return AllyExecuteResult(ResurrectResult(ResurrectStatus::PLAYER_RESURRECTED, type));
    }

    std::cout << "[Priest] El jugador ya está vivo" << std::endl;
    return AllyExecuteResult(ResurrectResult(ResurrectStatus::PLAYER_IS_ALIVE, type));
}
