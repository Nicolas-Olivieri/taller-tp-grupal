#include "sprite_creator.h"

#include <map>
#include <string>
#include <utility>

#include "sprite_layer.h"

#define HEAD_OFFSET 21

SpriteCreator::SpriteCreator(SDL2pp::Renderer& renderer):
        texture_pool(TexturePool(renderer)), animation_pool(AnimationPool()), renderer(renderer) {}


#define GHOST_HEAD_ID 0
#define GHOST_BODY_ID 0

Sprite SpriteCreator::create_user(const PlayerInfoDTO& playerInfo) {
    const SDL2pp::Point position(playerInfo.x, playerInfo.y);
    const AppearanceDTO& appearance_data = playerInfo.appearance;

    SpriteLayer head = create_sprite_layer("head", appearance_data.head);
    SpriteLayer body = create_sprite_layer("body", appearance_data.body, SDL2pp::Point(0, HEAD_OFFSET));

    const SDL2pp::Point size = body.frame.Union(head.frame).GetSize();

    Sprite sprite(std::move(body), position, Direction::IDLE, size);
    sprite.add_layer(Layer::HEAD, std::move(head));

    if (playerInfo.stats.current_health == 0)
        convert_to_ghost(sprite);

    return sprite;
}

SpriteLayer SpriteCreator::create_sprite_layer(const std::string& category, const uint8_t id,
                                               const SDL2pp::Point& offset) {
    SDL2pp::Texture& texture = texture_pool.get_sprite_texture(category, id);
    std::map<Direction, Animation>& actions = animation_pool.get_animation(category);

    return {renderer, texture, offset, actions};
}

void SpriteCreator::update_appearance(Sprite& player, const AppearanceDTO& appearance) {
    player.remove_all_layers();

    SpriteLayer head = create_sprite_layer("head", appearance.head);
    SpriteLayer body = create_sprite_layer("body", appearance.body, SDL2pp::Point(0, HEAD_OFFSET));

    player.add_layer(Layer::BODY, std::move(body));
    player.add_layer(Layer::HEAD, std::move(head));
}

void SpriteCreator::convert_to_ghost(Sprite& player) {
    player.remove_all_layers();

    SpriteLayer head = create_sprite_layer("head", GHOST_HEAD_ID);
    SpriteLayer body = create_sprite_layer("body", GHOST_BODY_ID, SDL2pp::Point(0, HEAD_OFFSET));

    player.add_layer(Layer::BODY, std::move(body));
    player.add_layer(Layer::HEAD, std::move(head));
}
