#include "killable.h"

#include "player/player.h"


Killable::Killable(const int attack_cooldown, const int move_cooldown, const int agility,
                   const int constitution, const int intelligence, const int strength,
                   const Archetype archetype, const Race race):
        is_meditating(false),
        attack_cooldown(attack_cooldown),
        move_cooldown(move_cooldown),
        stats(agility, constitution, intelligence, strength, archetype, race) {}


// TODO
void Killable::drop() {}


// TODO
void Killable::interact(Player&) {}
