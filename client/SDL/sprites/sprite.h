#ifndef SPRITE_H
#define SPRITE_H
#include "SDL2pp/SDL2pp.hh"

#include <memory>

#include "sprite_label.h"

class Sprite {
protected:
    SDL2pp::Point position;
    SDL2pp::Point size;
    SDL2pp::Point render_offset;

    SDL2pp::Point to_sprite_point(const SDL2pp::Point& point);

    std::unique_ptr<SpriteLabel> label;

public:
    Sprite(SDL2pp::Point position, SDL2pp::Point size, SDL2pp::Point render_offset);

    virtual ~Sprite() = default;

    virtual void update_frame(int iteration) = 0;

    virtual void render(const SDL2pp::Point& camera_offset) = 0;

    virtual bool intersects(const SDL2pp::Rect& area, const SDL2pp::Point& offset) const = 0;

    SDL2pp::Point get_position() const;

    SDL2pp::Point get_size() const;

    SDL2pp::Point get_ground_position() const;

    bool has_label() const;

    SpriteLabel& get_label() const;

    void set_label(std::unique_ptr<SpriteLabel> new_label);

    void render_overlay(const SDL2pp::Point& camera_offset) const;
};


#endif  // SPRITE_H
