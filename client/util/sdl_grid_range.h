#ifndef SDLGRIDRANGE_H
#define SDLGRIDRANGE_H
#include "SDL2pp/Point.hh"


class SDLGridRange {
private:
    SDL2pp::Point origin;
    int width;
    int height;


public:
    SDLGridRange(int origin_x, int origin_y, SDL2pp::Point size);

    SDLGridRange(SDL2pp::Point origin, SDL2pp::Point size);

    class Iterator {
    private:
        SDL2pp::Point origin;
        int curr_x;
        int curr_y;
        int width;

    public:
        using iterator_category = std::input_iterator_tag;
        using value_type = SDL2pp::Point;
        using difference_type = std::ptrdiff_t;
        using pointer = SDL2pp::Point*;
        using reference = SDL2pp::Point;

        Iterator(SDL2pp::Point origin, int width, int curr_x, int curr_y);

        Iterator& operator++();

        SDL2pp::Point operator*() const;

        bool operator!=(const Iterator& other) const;

        bool operator==(const Iterator& other) const;
    };

    Iterator begin() const;
    Iterator end() const;
};


#endif  //SDLGRIDRANGE_H
