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
        equipment({NO_ITEM, NO_ITEM, NO_ITEM, 1}),  // TODO: sacar hardcodeo
        position(position),
        direction(Direction::IDLE) {}

Killable::Killable(uint8_t race_id, uint8_t variation_id, uint8_t level, Position position):
        required_attack_cooldown(GameConfig::get().get_cooldown().attack),
        required_move_cooldown(GameConfig::get().get_cooldown().move),
        current_attack_cooldown(0),
        current_move_cooldown(0),
        is_meditating(false),
        stats(race_id, variation_id, level),
        equipment({NO_ITEM, NO_ITEM, NO_ITEM, 1}),  // TODO: sacar hardcodeo
        position(position),
        direction(Direction::IDLE) {}

bool Killable::can_move() const { return current_move_cooldown == 0; }


void Killable::update_position(const Position& new_position, const Direction& new_direction) {
    position = new_position;
    direction = new_direction;
    current_move_cooldown = required_move_cooldown;
}

uint16_t Killable::receive_damage(Attacker& attacker) {
    const uint16_t damage = attacker.attack();
    const uint16_t defense = Calculator::calculate_defense(equipment);

    const uint16_t damage_applied = damage > defense ? damage - defense : 0;

    // TODO: creo que este método puede dejar de ser bool
    stats.health.loose(damage_applied);

    return damage_applied;
}

InteractResult Killable::interact(Player& attacker) {
    if (not attacker.can_reach(position))
        return InteractResult(AttackStatus::OUT_OF_RANGE);

    if (not attacker.can_attack())
        return InteractResult(AttackStatus::CANNOT_ATTACK);

    if (Calculator::can_dodge(stats.agility))
        return InteractResult(AttackStatus::TARGET_DODGED);

    const uint16_t damage_applied = receive_damage(attacker);

    // TODO notificar el caso particular?
    if (damage_applied == 0)
        return InteractResult(0, false);

    bool was_killed = !is_alive();
    uint32_t earned_xp =
            was_killed ?
                    Calculator::kill_exp(this->stats.health.get_max(), this->stats.experience.get_level(),
                                         attacker.stats.experience.get_level()) :
                    Calculator::attack_exp(damage_applied, this->stats.experience.get_level(),
                                           attacker.stats.experience.get_level());
    attacker.earn_xp(earned_xp);

    return InteractResult(damage_applied, was_killed);
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
