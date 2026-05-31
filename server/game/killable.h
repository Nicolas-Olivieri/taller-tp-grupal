#ifndef KILLABLE_H
#define KILLABLE_H

#include "server/game/stats/stats.h"

#include "interactive.h"


class Player;


class Killable: public Interactive {
protected:
    bool is_meditating;
    int attack_cooldown;
    int move_cooldown;

    Stats stats;

    Killable(int attack_cooldown, int move_cooldown, uint8_t archetype_id, uint8_t race_id,
             uint32_t current_xp_amount, uint8_t level);

public:
    void drop();

    bool interact(Player&) override;

    int get_strength() const;
};


#endif  // KILLABLE_H
