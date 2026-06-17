#ifndef TELEPORT_RESULT_H
#define TELEPORT_RESULT_H

#include "common/npc_type.h"
#include "server/game/position.h"

enum class TeleportStatus {
    SUCCESS,
    GHOST_FAIL,
    DESTINATION_BLOCKED,
    ACTION_NOT_ACCEPTED,
    PLAYER_UNBOUNDED,
    NO_RESULT
};

struct TeleportResult {
    TeleportStatus status;
    AllyType ally;
    Position destination;

    TeleportResult();

    explicit TeleportResult(const TeleportStatus& status);

    TeleportResult(const TeleportStatus& status, const AllyType& type);

    TeleportResult(const TeleportStatus& status, const AllyType& ally, const Position& destination);
};


#endif  // TELEPORT_RESULT_H
