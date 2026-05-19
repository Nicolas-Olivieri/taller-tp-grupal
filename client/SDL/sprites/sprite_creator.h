#ifndef SPRITE_CREATOR_H
#define SPRITE_CREATOR_H

#include <string>

#include "common/dto/snapshot/actions/appearance.h"

#include "animation_pool.h"
#include "sprite.h"
#include "sprite_layer.h"
#include "texture_pool.h"


class SpriteCreator {
private:
    TexturePool texture_pool;
    AnimationPool animation_pool;
    SDL2pp::Renderer& renderer;

public:
    explicit SpriteCreator(SDL2pp::Renderer& renderer);

    SpriteLayer create_sprite_layer(
            const std::string& category, uint8_t id,
            const SDL2pp::Point& offset = SDL2pp::Point(0, 0));

    Sprite create_user(const AppearanceDTO& appearance_data);
};


#endif  // SPRITE_CREATOR_H
