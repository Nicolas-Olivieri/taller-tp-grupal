#include "effect_sprite.h"

EffectSprite::EffectSprite(SpriteLayer &&image, const SDL2pp::Point &position, const SDL2pp::Point &size) :
    FixedSprite (std::move(image), position, size), elapsed_frames(0) {}

void EffectSprite::update_frame(const int iteration) {
    FixedSprite::update_frame(iteration);
    elapsed_frames++;
}

bool EffectSprite::has_finished() const {
    return elapsed_frames >= image.get_animation_frame_amount();
}
