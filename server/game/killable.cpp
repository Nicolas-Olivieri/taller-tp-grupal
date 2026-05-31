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
    const int damage = Calculator::calculate_unarmed_damage(attacker.get_strength());
    return stats.health.loose(damage);
}


int Killable::get_strength() const { return stats.strength; }
