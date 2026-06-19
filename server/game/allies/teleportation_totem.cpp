#include "teleportation_totem.h"

#include <cassert>

#include "server/command/cmd_results/ally_execute/teleport/teleport_result.h"
#include "server/game/player/player.h"

TeleportationTotem::TeleportationTotem(const Position& position, const Position& peer_position):
        Ally(position, AllyType::TOTEM), peer_position(peer_position) {}

AllyExecuteResult TeleportationTotem::execute(Player& player, const AllyActionPayload& payload) const {
    switch (payload.action) {
        case AllyAction::BUY:
            return handle_action_not_accepted<BuyResult>(BuyStatus::ACTION_NOT_ACCEPTED);

        case AllyAction::DEPOSIT_GOLD:
            return handle_action_not_accepted<DepositGoldResult>(DepositGoldStatus::ACTION_NOT_ACCEPTED);

        case AllyAction::DEPOSIT_ITEM:
            return handle_action_not_accepted<DepositItemResult>(DepositItemStatus::ACTION_NOT_ACCEPTED);

        case AllyAction::HEAL:
            return handle_action_not_accepted<HealResult>(HealStatus::ACTION_NOT_ACCEPTED);

        case AllyAction::LIST_ITEMS:
            return handle_action_not_accepted<TeleportResult>(TeleportStatus::ACTION_NOT_ACCEPTED);

        case AllyAction::RESURRECT:
            return handle_action_not_accepted<ResurrectResult>(ResurrectStatus::ACTION_NOT_ACCEPTED);

        case AllyAction::SELL:
            return handle_action_not_accepted<SellResult>(SellStatus::ACTION_NOT_ACCEPTED);

        case AllyAction::WITHDRAW_GOLD:
            return handle_action_not_accepted<WithdrawGoldResult>(WithdrawGoldStatus::ACTION_NOT_ACCEPTED);

        case AllyAction::WITHDRAW_ITEM:
            return handle_action_not_accepted<WithdrawItemResult>(WithdrawItemStatus::ACTION_NOT_ACCEPTED);

        default:
            break;
    }

    assert(payload.action == AllyAction::TELEPORT);

    if (!player.is_alive())
        return AllyExecuteResult(TeleportResult(TeleportStatus::GHOST_FAIL, type, peer_position));

    return AllyExecuteResult(TeleportResult(TeleportStatus::SUCCESS, type, peer_position));
}
