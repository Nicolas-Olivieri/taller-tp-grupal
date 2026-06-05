#include "position.h"

#include <cmath>

Position::Position(const int x, const int y): x(x), y(y) {}


Position Position::operator+(const Position& other) const { return Position(x + other.x, y + other.y); }


bool Position::operator==(const Position& other) const { return x == other.x and y == other.y; }


Position Position::move(const Direction& direction) const {
    switch (direction) {
        case Direction::UP:
            return Position(x, y - 1);
        case Direction::DOWN:
            return Position(x, y + 1);
        case Direction::RIGHT:
            return Position(x + 1, y);
        case Direction::LEFT:
            return Position(x - 1, y);
        default:
            return *this;
    }
}


int Position::get_x() const { return x; }


int Position::get_y() const { return y; }

float Position::distance_to(const Position& position) const {
    return std::sqrt(std::pow(x - position.x, 2) + std::pow(y - position.y, 2));
}

std::ostream& operator<<(std::ostream& os, const Position& position) {
    os << "(" << position.x << ", " << position.y << ")";
    return os;
}
