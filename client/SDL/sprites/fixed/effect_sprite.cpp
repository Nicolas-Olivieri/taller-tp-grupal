#include "effect_sprite.h"

#include <utility>

#include "client/config/client_config.h"

EffectSprite::EffectSprite(SpriteLayer&& image, const SDL2pp::Point& position, const SDL2pp::Point& size):
        FixedSprite(std::move(image), position, size), elapsed_frames(0), finished(false) {
    const uint16_t tile_size = ClientConfig::get().get_tile_size();

    render_offset = SDL2pp::Point((size.x - tile_size) / 2, (size.y - tile_size) / 2);
}

void EffectSprite::update_frame(const int /* iteration*/) {
    FixedSprite::update_frame(elapsed_frames);
    elapsed_frames++;
}

bool EffectSprite::has_finished() const { return elapsed_frames >= image.get_animation_frame_amount(); }
