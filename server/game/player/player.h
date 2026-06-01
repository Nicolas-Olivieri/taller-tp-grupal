#ifndef PLAYER_H
#define PLAYER_H

#include <string>

#include "../killable.h"
#include "../position.h"
#include "server/persistance/playerdata.h"

class Player: public Killable {
private:
    const std::string player_name;
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

    void earn_xp(uint32_t amount);

    void update();
};


#endif  // PLAYER_H
