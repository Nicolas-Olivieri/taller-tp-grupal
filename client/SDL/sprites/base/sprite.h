#ifndef SPRITE_H
#define SPRITE_H
#include "SDL2pp/SDL2pp.hh"

class Sprite {
protected:
    SDL2pp::Point position;
    SDL2pp::Point size;
    SDL2pp::Point render_offset;

public:
    Sprite(SDL2pp::Point position, SDL2pp::Point size, SDL2pp::Point render_offset);

    virtual ~Sprite() = default;

    SDL2pp::Point get_position() const;

    SDL2pp::Point get_size() const;

    void set_visual_position(const SDL2pp::Point& new_visual_position);
};


#endif  // SPRITE_H
