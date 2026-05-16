#include "sprite_creator.h"

#include "sprite_layer.h"

#define HEAD_ID 1
#define BODY_ID 2

#define HEAD_OFFSET 21

SpriteCreator::SpriteCreator(SDL2pp::Renderer& renderer) :
    texture_pool(TexturePool(renderer)), animation_pool(AnimationPool()), renderer(renderer) {}


Sprite SpriteCreator::create_user() {
    SpriteLayer head = create_sprite_layer(HEAD_ID);
    SpriteLayer body = create_sprite_layer(BODY_ID);

    Sprite sprite(std::move(body), SDL2pp::Point(0,0), IDLE);
    sprite.add_layer(Layer::HEAD, std::move(head));

    return sprite;
}

SpriteLayer SpriteCreator::create_sprite_layer(const int id) {
    SDL2pp::Texture& texture = texture_pool.get_sprite_texture(id);
    std::map<Direction, Animation>& actions = animation_pool.get_animation(id);

    const SDL2pp::Rect base = texture_pool.get_sprite_rect(id);
    const SDL2pp::Point offset = id == 1 ? SDL2pp::Point(0,0) : SDL2pp::Point(0,HEAD_OFFSET);

    return SpriteLayer(renderer, texture, offset, base, actions);
}
