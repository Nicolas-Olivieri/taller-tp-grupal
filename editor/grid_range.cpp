#include "grid_range.h"

GridRange::Iterator::Iterator(const QPoint origin, const int width, const int curr_x, const int curr_y):
        origin(origin), curr_x(curr_x), curr_y(curr_y), width(width) {}

GridRange::Iterator& GridRange::Iterator::operator++() {
    curr_x++;
    if (curr_x >= width) {
        curr_x = 0;
        curr_y++;
    }
    return *this;
}

QPoint GridRange::Iterator::operator*() const { return origin + QPoint(curr_x, curr_y); }

bool GridRange::Iterator::operator!=(const Iterator& other) const {
    return curr_x != other.curr_x || curr_y != other.curr_y;
}

bool GridRange::Iterator::operator==(const Iterator& other) const {
    return curr_x == other.curr_x && curr_y == other.curr_y;
}


GridRange::GridRange(const QPoint origin, const int width, const int height):
        origin(origin), width(width), height(height) {}

GridRange::Iterator GridRange::begin() const { return Iterator(origin, width, 0, 0); }

GridRange::Iterator GridRange::end() const { return Iterator(origin, width, 0, height); }
