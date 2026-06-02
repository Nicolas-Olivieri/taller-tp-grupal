#include "sprite_creator.h"

#include <map>
#include <string>
#include <utility>

#include "sprite_layer.h"

#define HEAD_OFFSET 21

SpriteCreator::SpriteCreator(SDL2pp::Renderer& renderer):
        texture_pool(TexturePool(renderer)), animation_pool(AnimationPool()), renderer(renderer) {}


#define GHOST_HEAD_ID 4
#define GHOST_BODY_ID 3

Sprite SpriteCreator::create_user(const PlayerInfoDTO& playerInfo) {
    const SDL2pp::Point position(playerInfo.x, playerInfo.y);

    const AppearanceDTO& appearance_data = playerInfo.appearance;
    uint8_t head_id = appearance_data.head;
    uint8_t body_id = appearance_data.body;

    if (playerInfo.stats.current_health == 0) {
        head_id = GHOST_HEAD_ID;
        body_id = GHOST_BODY_ID;
    }

    SpriteLayer head = create_sprite_layer("head", head_id);
    SpriteLayer body = create_sprite_layer("body", body_id, SDL2pp::Point(0, HEAD_OFFSET));

    const SDL2pp::Point size = body.frame.Union(head.frame).GetSize();

    Sprite sprite(std::move(body), position, Direction::IDLE, size);
    sprite.add_layer(Layer::HEAD, std::move(head));

    return sprite;
}

SpriteLayer SpriteCreator::create_sprite_layer(const std::string& category, const uint8_t id,
                                               const SDL2pp::Point& offset) {
    SDL2pp::Texture& texture = texture_pool.get_sprite_texture(category, id);
    std::map<Direction, Animation>& actions = animation_pool.get_animation(category);

    return {renderer, texture, offset, actions};
}
