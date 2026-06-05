#include "updatestatus.h"

CreatureUpdateStatus::CreatureUpdateStatus():
        creature_id(0), did_attack(false), player_name(""), damage_dealt(0), killed_target(0) {}

CreatureUpdateStatus::CreatureUpdateStatus(uint8_t id, std::string player_name, uint16_t damage_dealt,
                                           bool killed_target):
        creature_id(id),
        did_attack(true),
        player_name(player_name),
        damage_dealt(damage_dealt),
        killed_target(killed_target) {}
