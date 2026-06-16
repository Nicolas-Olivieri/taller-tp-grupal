#include "teleportation_totem.h"

#include <cassert>

#include "server/game/player/player.h"

TeleportationTotem::TeleportationTotem(const Position& position, const Position& peer_position):
        Ally(position, AllyType::TOTEM), peer_position(peer_position) {
    assert(false);
}
AllyExecuteResult TeleportationTotem::execute(Player& /* player */,
                                              const AllyActionPayload& /* payload */) const {
    assert(false);

    return AllyExecuteResult(false);
}
