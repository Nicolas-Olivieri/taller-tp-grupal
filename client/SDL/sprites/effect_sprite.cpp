#include "effect_sprite.h"

#include <utility>

#include "client/client_constants.h"

EffectSprite::EffectSprite(SpriteLayer&& image, const SDL2pp::Point& position, const SDL2pp::Point& size):
        FixedSprite(std::move(image), position, size), elapsed_frames(0), finished(false) {
    render_offset = SDL2pp::Point((size.x - TILE_SIZE) / 2, (size.y - TILE_SIZE) / 2);
}

void EffectSprite::update_frame(const int iteration) {
    FixedSprite::update_frame(iteration);
    elapsed_frames++;
}

bool EffectSprite::has_finished() const { return elapsed_frames >= image.get_animation_frame_amount(); }
