#ifndef FIXED_SPRITE_H
#define FIXED_SPRITE_H
#include "sprite.h"
#include "sprite_layer.h"

class FixedSprite: public Sprite {
protected:
    SpriteLayer image;

public:
    FixedSprite(SpriteLayer&& image, const SDL2pp::Point& position, const SDL2pp::Point& size);

    virtual void update_frame(int iteration) override;

    virtual void render(const SDL2pp::Point& camera_offset) override;

    virtual bool intersects(const SDL2pp::Rect& area, const SDL2pp::Point& offset) const override;
};


#endif  // FIXED_SPRITE_H
