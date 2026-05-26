#include "killable.h"

#include "player/player.h"
#include "server/util/calculator.h"


Killable::Killable(const int attack_cooldown, const int move_cooldown, const int agility,
                   const int constitution, const int intelligence, const int strength,
                   const Archetype archetype, const Race race):
        is_meditating(false),
        attack_cooldown(attack_cooldown),
        move_cooldown(move_cooldown),
        stats(agility, constitution, intelligence, strength, archetype, race) {}


// TODO
void Killable::drop() {}


bool Killable::interact(Player& attacker) {
    const int damage = Calculator::calculate_unarmed_damage(attacker.get_strength());
    return stats.health.loose(damage);
}


int Killable::get_strength() const { return stats.strength; }
