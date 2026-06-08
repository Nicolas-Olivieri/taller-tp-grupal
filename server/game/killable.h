#ifndef KILLABLE_H
#define KILLABLE_H

#include <vector>

#include "server/game/items/equipment.h"
#include "server/game/loot.h"
#include "server/game/stats/stats.h"

#include "attacker.h"
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
    Equipment equipment;

    Position position;
    Direction direction;

    Killable(uint8_t archetype_id, uint8_t race_id, uint32_t current_xp_amount, uint8_t level,
             Position position, const Equipment& equipment);

    Killable(uint8_t race_id, uint8_t variation_id, uint8_t level, Position position,
             const Equipment& equipment);

public:
    virtual std::vector<Loot> drop() = 0;

    virtual bool can_move() const;

    virtual void update_position(const Position& new_position, const Direction& new_direction);

    InteractResult interact(Player&) override;

    Position get_position() const;

    Direction get_direction() const;

    bool is_alive() const;

    virtual void update();

    uint16_t receive_damage(Attacker& attacker);

    virtual ~Killable() = default;
};


#endif  // KILLABLE_H
