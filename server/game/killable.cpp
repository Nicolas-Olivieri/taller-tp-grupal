#include "killable.h"

#include <cassert>

#include "player/player.h"
#include "server/config/game_config.h"
#include "server/util/calculator.h"


Killable::Killable(uint8_t archetype_id, uint8_t race_id, uint32_t current_xp_amount, uint8_t level,
                   Position position, const Equipment& equipment, const char* clan_ptr, bool is_clan_founder):
        required_attack_cooldown(GameConfig::get().get_player_cooldown().attack),
        required_move_cooldown(GameConfig::get().get_player_cooldown().move),
        current_attack_cooldown(0),
        current_move_cooldown(0),
        is_meditating(false),
        stats(archetype_id, race_id, current_xp_amount, level),
        equipment(equipment),
        position(position),
        direction(Direction::DOWN),
        clan() {
    if (clan_ptr != nullptr)
        clan = ClanMembership(clan_ptr, is_clan_founder);
}

Killable::Killable(uint8_t race_id, uint8_t variation_id, uint8_t level, Position position,
                   const Equipment& equipment):
        required_attack_cooldown(GameConfig::get().get_creature_cooldown().attack),
        required_move_cooldown(GameConfig::get().get_creature_cooldown().move),
        current_attack_cooldown(0),
        current_move_cooldown(0),
        is_meditating(false),
        stats(race_id, variation_id, level),
        equipment(equipment),
        position(position),
        direction(Direction::IDLE),
        clan() {}

bool Killable::can_move() const { return current_move_cooldown == 0; }


void Killable::update_position(const Position& new_position, const Direction& new_direction) {
    position = new_position;
    direction = new_direction;
    current_move_cooldown = required_move_cooldown;
}


uint16_t Killable::receive_damage(uint16_t damage) {
    const uint16_t defense = Calculator::calculate_defense(equipment, clan.get_clan_buff_factor());
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

    const uint16_t damage = attacker.attack();

    if (Calculator::can_dodge(stats.agility))
        return InteractResult(AttackStatus::TARGET_DODGED);

    const uint16_t damage_applied = receive_damage(damage);
    is_meditating = false;

    // TODO notificar el caso particular?
    if (damage_applied == 0)
        return InteractResult(attacker.get_equipment().weapon, 0, false);

    bool was_killed = !is_alive();
    uint32_t earned_xp =
            was_killed ?
                    Calculator::kill_exp(this->stats.health.get_max(), this->stats.experience.get_level(),
                                         attacker.stats.experience.get_level()) :
                    Calculator::attack_exp(damage_applied, this->stats.experience.get_level(),
                                           attacker.stats.experience.get_level());
    attacker.earn_xp(earned_xp);

    return InteractResult(attacker.get_equipment().weapon, damage_applied, was_killed);
}

const Position& Killable::get_position() const { return position; }


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

    stats.health.update();
    if (stats.health.get_current() == 0) {
        stats.mana.loose(stats.mana.get_current());
        return;
    }

    if (is_meditating) {
        stats.mana.meditate();
    } else {
        stats.mana.update();
    }
}

bool Killable::is_alive() const { return stats.health.get_current() > 0; }
