#ifndef PLAYER_H
#define PLAYER_H

#include <string>

#include "../killable.h"
#include "../position.h"
#include "server/game/stats/archetype.h"
#include "server/game/stats/race.h"


class Player: public Killable {
private:
    const std::string player_name;
    const Archetype archetype;
    const Race race;

    Position position;
    Direction direction;

public:
    explicit Player(const std::string& player_name, const Position& position);

    void update_position(const Position& new_position, const Direction& new_direction);

    void attack();

    Position get_position() const;

    Direction get_direction() const;

    bool can_attack() const;

    bool can_move() const;

    void update();
};


#endif  // PLAYER_H
