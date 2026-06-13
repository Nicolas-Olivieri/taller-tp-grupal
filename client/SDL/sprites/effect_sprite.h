#ifndef EFFECT_SPRITE_H
#define EFFECT_SPRITE_H
#include "fixed_sprite.h"


class EffectSprite: public FixedSprite {
private:
    int elapsed_frames;
    bool finished;

public:
    EffectSprite(SpriteLayer&& image, const SDL2pp::Point& position, const SDL2pp::Point& size);

    bool has_finished() const;

    virtual void update_frame(int iteration) override;

    bool operator<(const EffectSprite& other) const;
};


#endif  // EFFECT_SPRITE_H
