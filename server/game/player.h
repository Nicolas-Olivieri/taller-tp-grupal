#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <utility>

#include "killable.h"
#include "position.h"


class Player: public Killable {
private:
    std::string player_name;
    Position position;
    Direction direction;

public:
    explicit Player(const std::string& player_name, const Position& position);

    void move(const Position& new_position, const Direction& new_direction);

    Position get_position() const;

    Direction get_direction() const;
};


#endif  // PLAYER_H
