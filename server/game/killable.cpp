#include "killable.h"

#include "player/player.h"
#include "server/util/calculator.h"


Killable::Killable(int attack_cooldown, int move_cooldown, uint8_t archetype_id, uint8_t race_id,
                   uint32_t current_xp_amount, uint8_t level):
        is_meditating(false),
        attack_cooldown(attack_cooldown),
        move_cooldown(move_cooldown),
        stats(archetype_id, race_id, current_xp_amount, level) {}


// TODO
void Killable::drop() {}


bool Killable::interact(Player& attacker) {
    const uint16_t damage = Calculator::calculate_unarmed_damage(attacker.get_strength());

    uint32_t earned_xp;
    bool was_killed = stats.health.loose(damage);

    if (was_killed)
        earned_xp = Calculator::kill_exp(this->stats.health.get_max(), this->stats.experience.get_level(),
                                         attacker.stats.experience.get_level());
    else
        earned_xp = Calculator::attack_exp(damage, this->stats.experience.get_level(),
                                           attacker.stats.experience.get_level());

    attacker.earn_xp(earned_xp);

    return was_killed;
}


int Killable::get_strength() const { return stats.strength; }
