#ifndef POSITION_H
#define POSITION_H

#include <iostream>


class Position {
private:
    int x;
    int y;

public:
    explicit Position(int x, int y);

    Position operator+(const Position& other) const;

    bool operator==(const Position& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Position& position);
};


std::ostream& operator<<(std::ostream& os, const Position& position);


#endif  // POSITION_H
