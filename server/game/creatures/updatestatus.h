#ifndef CREATUREUPDATE_H
#define CREATUREUPDATE_H

#include <cstdint>
#include <string>

#include "server/command/cmd_results/interact/attack_result.h"

enum class CreatureStatus { MOVING, WAITING, ATTACKED };

struct CreatureUpdate {
    CreatureStatus status;
    uint8_t creature_id;
    AttackResult attack;
    uint16_t x;
    uint16_t y;

    explicit CreatureUpdate(const CreatureStatus& status);

    CreatureUpdate(uint8_t id, const std::string& player_name, uint16_t damage_dealt, bool killed_target,
                   uint8_t weapon, uint16_t x, uint16_t y);
};

#endif  // CREATUREUPDATE_H
