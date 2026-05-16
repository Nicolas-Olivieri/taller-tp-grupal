#ifndef SPRITE_CREATOR_H
#define SPRITE_CREATOR_H

#include "sprite_layer.h"

#include "sprite.h"
#include "texture_pool.h"
#include "animation_pool.h"


class SpriteCreator {
private:
    TexturePool texture_pool;
    AnimationPool animation_pool;
    SDL2pp::Renderer& renderer;

public:
    explicit SpriteCreator(SDL2pp::Renderer& renderer);

    SpriteLayer create_sprite_layer(int id);

    Sprite create_user();
};


#endif //SPRITE_CREATOR_H
