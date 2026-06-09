#ifndef POSITION_H
#define POSITION_H

#include <iostream>
#include <utility>

#include "common/direction.h"


class Position {
private:
    int x;
    int y;

public:
    explicit Position(int x, int y);

    Position operator+(const Position& other) const;

    bool operator==(const Position& other) const;

    bool operator<(const Position& other) const;

    Position move(const Direction& direction) const;

    int get_x() const;

    int get_y() const;

    float distance_to(const Position& position) const;

    friend std::ostream& operator<<(std::ostream& os, const Position& position);
};


std::ostream& operator<<(std::ostream& os, const Position& position);


#endif  // POSITION_H
