#include "teleportation_totem.h"

#include <cassert>

#include "server/command/cmd_results/ally_execute/teleport/teleport_result.h"
#include "server/game/player/player.h"

TeleportationTotem::TeleportationTotem(const Position& position, const Position& peer_position):
        Ally(position, AllyType::TOTEM), peer_position(peer_position) {}

AllyExecuteResult TeleportationTotem::execute(Player& player, const AllyActionPayload& payload) const {
    if (payload.action != AllyAction::TELEPORT)
        return handle_action_not_accepted<TeleportResult>(TeleportStatus::ACTION_NOT_ACCEPTED);

    if (!player.is_alive())
        return AllyExecuteResult(TeleportResult(TeleportStatus::GHOST_FAIL, type, peer_position));

    player.unbind_ally();

    return AllyExecuteResult(TeleportResult(TeleportStatus::SUCCESS, type, peer_position));
}
