#include "updatestatus.h"

CreatureUpdate::CreatureUpdate(const CreatureStatus& status):
        status(status), creature_id(0), player_name(""), damage_dealt(0), killed_target(0) {}

CreatureUpdate::CreatureUpdate(uint8_t id, std::string player_name, uint16_t damage_dealt,
                               bool killed_target):
        status(CreatureStatus::ATTACKED),
        creature_id(id),
        player_name(player_name),
        damage_dealt(damage_dealt),
        killed_target(killed_target) {}
