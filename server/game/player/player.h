#ifndef PLAYER_H
#define PLAYER_H

#include <string>

#include "../killable.h"
#include "../position.h"
#include "server/persistance/playerdata.h"
/* #include "server/game/stats/archetype.h"
#include "server/game/stats/race.h" */

class Player: public Killable {
private:
    const std::string player_name;
    // TODO: ver si es necesario que el player conozca esto por afuera de cómo lo maneja al heredar de
    // Killable const Archetype archetype; const Race race;
    const uint8_t body;
    const uint8_t head;

    Position position;
    Direction direction;

public:
    Player(const std::string& player_name, const PlayerData& persisted_data);

    Player(const std::string& player_name, const PlayerData& persisted_data,
           const Position& starting_position);

    void update_position(const Position& new_position, const Direction& new_direction);

    void attack();

    Stats get_stats() const;

    Position get_position() const;

    Direction get_direction() const;

    uint8_t get_body() const;

    uint8_t get_head() const;

    bool can_attack() const;

    bool can_move() const;

    void update();
};


#endif  // PLAYER_H
