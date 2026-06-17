#ifndef WORLD_SPRITE_H
#define WORLD_SPRITE_H
#include "sprite.h"


class WorldSprite: public Sprite {
protected:
    SDL2pp::Point to_sprite_point(const SDL2pp::Point& point);

public:
    WorldSprite(SDL2pp::Point position, SDL2pp::Point size, SDL2pp::Point render_offset);

    virtual void update_frame(int iteration) = 0;

    virtual void render(const SDL2pp::Point& camera_offset) = 0;

    virtual bool intersects(const SDL2pp::Rect& area, const SDL2pp::Point& offset) const = 0;

    SDL2pp::Point get_ground_position() const;
};


#endif  // WORLD_SPRITE_H
