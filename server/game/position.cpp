#include "position.h"


Position::Position(const int x, const int y): x(x), y(y) {}


Position Position::operator+(const Position& other) const {
    return Position(x + other.x, y + other.y);
}


bool Position::operator==(const Position& other) const {
    return x == other.x and y == other.y;
}


std::ostream& operator<<(std::ostream& os, const Position& position) {
    os << "(" << position.x << ", " << position.y << ")";
    return os;
}
