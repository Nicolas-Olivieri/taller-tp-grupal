#include "sprite_creator.h"

#include <map>
#include <string>
#include <utility>

#include "sprite_layer.h"

#define HEAD_OFFSET 21

SpriteCreator::SpriteCreator(SDL2pp::Renderer& renderer):
        texture_pool(TexturePool(renderer)), animation_pool(AnimationPool()), renderer(renderer) {}


Sprite SpriteCreator::create_user(const AppearanceDTO& appearance_data) {
    SpriteLayer head = create_sprite_layer("head", appearance_data.head);
    SpriteLayer body = create_sprite_layer("body", appearance_data.body, SDL2pp::Point(0, HEAD_OFFSET));

    const SDL2pp::Point size = body.frame.Union(head.frame).GetSize();

    Sprite sprite(std::move(body), SDL2pp::Point(0, 0), Direction::IDLE, size);
    sprite.add_layer(Layer::HEAD, std::move(head));

    return sprite;
}

SpriteLayer SpriteCreator::create_sprite_layer(const std::string& category, const uint8_t id,
                                               const SDL2pp::Point& offset) {
    SDL2pp::Texture& texture = texture_pool.get_sprite_texture(category, id);
    std::map<Direction, Animation>& actions = animation_pool.get_animation(category);

    return {renderer, texture, offset, actions};
}
