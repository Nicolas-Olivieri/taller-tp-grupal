#ifndef PLAYER_H
#define PLAYER_H

#include <string>

#include "position.h"


class Player {
private:
    std::string player_name;
    Position position;

public:
    explicit Player(const std::string& player_name, const Position& position);

    void move(const Position& new_position);

    Position get_position() const;
};


#endif  // PLAYER_H
