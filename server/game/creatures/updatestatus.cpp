#include "updatestatus.h"

CreatureUpdate::CreatureUpdate(const CreatureStatus& status):
        status(status), creature_id(), attack(), x(), y() {}

CreatureUpdate::CreatureUpdate(uint8_t id, const std::string& player_name, uint16_t damage_dealt,
                               bool killed_target, uint8_t weapon, uint16_t x, uint16_t y):
        status(CreatureStatus::ATTACKED), creature_id(id), x(x), y(y) {
    attack.status = damage_dealt > 0 ? AttackStatus::HIT : AttackStatus::TARGET_DODGED;
    attack.player_attacked = player_name;
    attack.damage_dealt = damage_dealt;
    attack.was_killed = killed_target;
    attack.weapon = weapon;
}
