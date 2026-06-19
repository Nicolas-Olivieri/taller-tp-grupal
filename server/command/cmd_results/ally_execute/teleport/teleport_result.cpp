#include "teleport_result.h"

TeleportResult::TeleportResult():
        status(TeleportStatus::NO_RESULT), ally(AllyType::NO_ALLY), destination(0, 0) {}


TeleportResult::TeleportResult(const TeleportStatus& status):
        status(status), ally(AllyType::NO_ALLY), destination(0, 0) {}

TeleportResult::TeleportResult(const TeleportStatus& status, const AllyType& type):
        status(status), ally(type), destination(0, 0) {}

TeleportResult::TeleportResult(const TeleportStatus& status, const AllyType& ally,
                               const Position& destination):
        status(status), ally(ally), destination(destination) {}
