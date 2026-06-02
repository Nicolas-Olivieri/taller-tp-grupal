#include "killable.h"

#include <cassert>

#include "player/player.h"
#include "server/config/game_config.h"
#include "server/util/calculator.h"


Killable::Killable(const uint8_t archetype_id, const uint8_t race_id, const uint32_t current_xp_amount,
                   const uint8_t level, const Position position):
        required_attack_cooldown(GameConfig::get().get_cooldown().attack),
        required_move_cooldown(GameConfig::get().get_cooldown().move),
        current_attack_cooldown(0),
        current_move_cooldown(0),
        is_meditating(false),
        stats(archetype_id, race_id, current_xp_amount, level),
        position(position),
        direction(Direction::IDLE) {}

bool Killable::can_move() const { return current_move_cooldown == 0; }


void Killable::update_position(const Position& new_position, const Direction& new_direction) {
    position = new_position;
    direction = new_direction;
    current_move_cooldown = required_move_cooldown;
}

Position Killable::get_position() const { return position; }


Direction Killable::get_direction() const { return direction; }

void Killable::update() {
    current_attack_cooldown--;
    if (current_attack_cooldown <= 0) {
        current_attack_cooldown = 0;
    }

    current_move_cooldown--;
    if (current_move_cooldown <= 0) {
        current_move_cooldown = 0;
    }
}

bool Killable::is_alive() const { return stats.health.get_current() > 0; }
