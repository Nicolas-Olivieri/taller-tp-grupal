#ifndef KILLABLE_H
#define KILLABLE_H

#include "server/game/stats/stats.h"

#include "interactive.h"
#include "position.h"


class Player;


class Killable: public Interactive {
protected:
    const int required_attack_cooldown;
    const int required_move_cooldown;

    int current_attack_cooldown;
    int current_move_cooldown;

    bool is_meditating;

    Stats stats;

    Position position;
    Direction direction;

    Killable(uint8_t archetype_id, uint8_t race_id, uint32_t current_xp_amount, uint8_t level,
             Position position);

public:
    virtual void drop() = 0;

    bool can_move() const;

    void update_position(const Position& new_position, const Direction& new_direction);

    Position get_position() const;

    Direction get_direction() const;

    virtual void update();

    virtual ~Killable() = default;
};


#endif  // KILLABLE_H
