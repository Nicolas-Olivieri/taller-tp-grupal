#include "killable.h"

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


// TODO
void Killable::drop() {}


bool Killable::interact(Player& attacker) {
    const uint16_t damage = Calculator::calculate_unarmed_damage(attacker.get_strength());

    uint32_t earned_xp;
    const bool was_killed = stats.health.loose(damage);

    if (was_killed)
        earned_xp = Calculator::kill_exp(this->stats.health.get_max(), this->stats.experience.get_level(),
                                         attacker.stats.experience.get_level());
    else
        earned_xp = Calculator::attack_exp(damage, this->stats.experience.get_level(),
                                           attacker.stats.experience.get_level());

    attacker.earn_xp(earned_xp);

    return was_killed;
}


bool Killable::can_move() const { return current_move_cooldown == 0; }


void Killable::update_position(const Position& new_position, const Direction& new_direction) {
    position = new_position;
    direction = new_direction;
    current_move_cooldown = required_move_cooldown;
}


bool Killable::can_attack() const { return current_attack_cooldown == 0; }


void Killable::attack() { current_attack_cooldown = required_attack_cooldown; }


int Killable::get_strength() const { return stats.strength; }


Position Killable::get_position() const { return position; }


Direction Killable::get_direction() const { return direction; }
