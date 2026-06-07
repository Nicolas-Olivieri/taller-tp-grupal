#include "sprite_creator.h"

#include <map>
#include <utility>

#include "common/dto/snapshot/map/asset_info.h"

#include "sprite_layer.h"

#define HEAD_OFFSET 21

SpriteCreator::SpriteCreator(SDL2pp::Renderer& renderer):
        texture_pool(TexturePool(renderer)), animation_pool(AnimationPool()), renderer(renderer) {}


#define GHOST_HEAD_ID 0
#define GHOST_BODY_ID 0

Sprite SpriteCreator::create_sprite(const PlayerInfoDTO& player_info) {
    const SDL2pp::Point position(player_info.x, player_info.y);
    const AppearanceDTO& appearance_data = player_info.appearance;

    SpriteLayer head = create_sprite_layer(SpriteCategory::HEAD, appearance_data.head);
    SpriteLayer body =
            create_sprite_layer(SpriteCategory::BODY, appearance_data.body, SDL2pp::Point(0, HEAD_OFFSET));

    SDL2pp::Rect head_rect(head.offset, head.frame.GetSize());
    SDL2pp::Rect body_rect(body.offset, body.frame.GetSize());
    const SDL2pp::Point size = body_rect.Union(head_rect).GetSize();

    Sprite sprite(std::move(body), position, player_info.direction, size);
    sprite.add_layer(Layer::HEAD, std::move(head));

    if (player_info.stats.current_health == 0)
        convert_to_ghost(sprite);

    return sprite;
}

Sprite SpriteCreator::create_sprite(const CreatureInfoDTO& creature_info) {
    const SDL2pp::Point position(creature_info.x, creature_info.y);
    const auto category = static_cast<SpriteCategory>(creature_info.creature);

    SpriteLayer creature = create_sprite_layer(category, creature_info.variation);
    const SDL2pp::Point size = creature.frame.GetSize();

    Sprite sprite(std::move(creature), position, creature_info.direction, size);
    return sprite;
}

Sprite SpriteCreator::create_sprite(const SpriteCategory category, const AssetInfoDTO& asset_info) {
    const SDL2pp::Point position(asset_info.x, asset_info.y);
    SpriteLayer base = create_sprite_layer(category, asset_info.id);
    const SDL2pp::Point size = base.frame.GetSize();

    Sprite asset(std::move(base), position, size);
    return asset;
}

SpriteLayer SpriteCreator::create_sprite_layer(const SpriteCategory category, const uint8_t id,
                                               const SDL2pp::Point& offset) {
    SDL2pp::Texture& texture = texture_pool.get_sprite_texture(category, id);

    switch (category) {
        case SpriteCategory::NPC:
        case SpriteCategory::TILE:
        case SpriteCategory::COLLIDER: {
            const Animation action = animation_pool.get_item_animation(category, id);
            return SpriteLayer(renderer, texture, offset, action);
        }
        default: {
            auto actions = animation_pool.get_walking_animations(category);
            return SpriteLayer(renderer, texture, offset, actions);
        }
    }
}

void SpriteCreator::update_appearance(Sprite& player, const AppearanceDTO& appearance) {
    player.remove_all_layers();

    SpriteLayer head = create_sprite_layer(SpriteCategory::HEAD, appearance.head);
    SpriteLayer body =
            create_sprite_layer(SpriteCategory::BODY, appearance.body, SDL2pp::Point(0, HEAD_OFFSET));

    player.add_layer(Layer::BODY, std::move(body));
    player.add_layer(Layer::HEAD, std::move(head));
}

void SpriteCreator::convert_to_ghost(Sprite& player) {
    player.remove_all_layers();

    SpriteLayer head = create_sprite_layer(SpriteCategory::HEAD, GHOST_HEAD_ID);
    SpriteLayer body =
            create_sprite_layer(SpriteCategory::BODY, GHOST_BODY_ID, SDL2pp::Point(0, HEAD_OFFSET));

    player.add_layer(Layer::BODY, std::move(body));
    player.add_layer(Layer::HEAD, std::move(head));
}
