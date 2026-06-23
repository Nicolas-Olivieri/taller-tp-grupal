#include "sdl_grid_range.h"

SDLGridRange::Iterator::Iterator(const SDL2pp::Point origin, const int width, const int curr_x, const int curr_y):
        origin(origin), curr_x(curr_x), curr_y(curr_y), width(width) {}

SDLGridRange::Iterator& SDLGridRange::Iterator::operator++() {
    curr_x++;
    if (curr_x >= width) {
        curr_x = 0;
        curr_y++;
    }
    return *this;
}

SDL2pp::Point SDLGridRange::Iterator::operator*() const { return origin + SDL2pp::Point(curr_x, curr_y); }

bool SDLGridRange::Iterator::operator!=(const Iterator& other) const {
    return curr_x != other.curr_x || curr_y != other.curr_y;
}

bool SDLGridRange::Iterator::operator==(const Iterator& other) const {
    return curr_x == other.curr_x && curr_y == other.curr_y;
}


SDLGridRange::SDLGridRange(const int origin_x, const int origin_y, const SDL2pp::Point size):
        origin(SDL2pp::Point(origin_x, origin_y)), width(size.x), height(size.y) {}

SDLGridRange::SDLGridRange(const SDL2pp::Point origin, const SDL2pp::Point size):
        origin(origin), width(size.x), height(size.y) {}

SDLGridRange::Iterator SDLGridRange::begin() const { return Iterator(origin, width, 0, 0); }

SDLGridRange::Iterator SDLGridRange::end() const { return Iterator(origin, width, 0, height); }
