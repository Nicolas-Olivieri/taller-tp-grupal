#ifndef CREATUREUPDATE_H
#define CREATUREUPDATE_H

#include <cstdint>
#include <string>

enum class CreatureStatus { MOVING, WAITING, ATTACKED };

struct CreatureUpdate {
    CreatureStatus status;
    uint8_t creature_id;
    std::string player_name;
    uint16_t damage_dealt;
    bool killed_target;

    explicit CreatureUpdate(const CreatureStatus& status);

    CreatureUpdate(uint8_t id, std::string player_name, uint16_t damage_dealt, bool killed_target);
};

#endif  // CREATUREUPDATE_H
