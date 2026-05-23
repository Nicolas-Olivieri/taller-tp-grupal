#include "killable.h"


Killable::Killable(const int attack_cooldown, const int move_cooldown):
        attack_cooldown(attack_cooldown), move_cooldown(move_cooldown) {}


// TODO
void Killable::drop() {}


// TODO
void Killable::interact(Player&) {}
