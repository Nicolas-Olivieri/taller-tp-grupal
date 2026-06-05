#ifndef CREATUREUPDATESTATUS_H
#define CREATUREUPDATESTATUS_H

#include <cstdint>
#include <string>

struct CreatureUpdateStatus {
    uint8_t creature_id;
    bool did_attack;
    std::string player_name;
    uint16_t damage_dealt;
    bool killed_target;

    CreatureUpdateStatus();

    CreatureUpdateStatus(uint8_t id, std::string player_name, uint16_t damage_dealt, bool killed_target);
};

#endif  // CREATUREUPDATESTATUS_H
