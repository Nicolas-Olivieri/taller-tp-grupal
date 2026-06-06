#include "sprite_creator.h"

#include <map>
#include <string>
#include <unordered_map>
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

    SDL2pp::Rect head_rect(head.offset, head.frame.GetSize());
    SDL2pp::Rect body_rect(body.offset, body.frame.GetSize());
    const SDL2pp::Point size = body_rect.Union(head_rect).GetSize();

    Sprite sprite(std::move(body), position, playerInfo.direction, size);
    sprite.add_layer(Layer::HEAD, std::move(head));

    if (playerInfo.stats.current_health == 0)
        convert_to_ghost(sprite);

    return sprite;
}

Sprite SpriteCreator::create_creature(const CreatureInfoDTO& creature_info) {
    const SDL2pp::Point position(creature_info.x, creature_info.y);

    // TODO: Esto podría estar en un config
    static std::unordered_map<uint8_t, std::string> creature_id_to_category = {
            {0, "goblin"}, {1, "skeleton"}, {2, "zombie"}, {3, "spider"}, {4, "orc"}, {5, "golem"},
    };

    const std::string& category = creature_id_to_category.at(creature_info.creature);
    SpriteLayer creature = create_sprite_layer(category, creature_info.variation);
    const SDL2pp::Point size = creature.frame.GetSize();

    Sprite sprite(std::move(creature), position, creature_info.direction, size);

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
