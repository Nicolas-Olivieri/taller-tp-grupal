#ifndef FIXED_SPRITE_H
#define FIXED_SPRITE_H
#include "../base/sprite_layer.h"
#include "client/SDL/sprites/base/world_sprite.h"

class FixedSprite: public WorldSprite {
protected:
    SpriteLayer image;

public:
    FixedSprite(SpriteLayer&& image, const SDL2pp::Point& position, const SDL2pp::Point& size);

    virtual void update_frame(int iteration) override;

    virtual void render(const SDL2pp::Point& camera_offset) override;

    virtual bool intersects(const SDL2pp::Rect& area, const SDL2pp::Point& offset) const override;
};


#endif  // FIXED_SPRITE_H
