#include "sprite_creator.h"

#include <map>
#include <ranges>
#include <utility>

#include "../sprites/sprite_layer.h"
#include "client/SDL/sprites/effect_sprite.h"
#include "client/SDL/sprites/enemy_sprite.h"
#include "client/SDL/sprites/fixed_sprite.h"
#include "client/SDL/sprites/player_sprite.h"
#include "client/client_constants.h"
#include "common/dto/snapshot/actions/action.h"
#include "common/dto/snapshot/map/asset_info.h"

SpriteCreator::SpriteCreator(SDL2pp::Renderer& renderer):
        texture_pool(TexturePool(renderer)), animation_pool(AnimationPool()), renderer(renderer) {}


PlayerSprite SpriteCreator::create_sprite(const PlayerInfoDTO& player_info) {
    const SDL2pp::Point position(player_info.x, player_info.y);
    const AppearanceDTO& appearance_data = player_info.appearance;

    SpriteLayer head = create_sprite_layer(SpriteCategory::HEAD, appearance_data.head);
    head.update_frame(0, player_info.direction);

    auto offset = get_layer_offset(Layer::BODY);
    SpriteLayer body = create_sprite_layer(SpriteCategory::BODY, appearance_data.body, offset);
    body.update_frame(0, player_info.direction);

    SDL2pp::Rect head_rect(head.offset, head.frame.GetSize());
    SDL2pp::Rect body_rect(body.offset, body.frame.GetSize());
    const SDL2pp::Point size = body_rect.Union(head_rect).GetSize();

    PlayerSprite sprite(std::move(head), std::move(body), position, size, player_info.direction);

    if (player_info.stats.current_health == 0)
        convert_to_ghost(sprite);

    return sprite;
}

EnemySprite SpriteCreator::create_sprite(const CreatureInfoDTO& creature_info) {
    const SDL2pp::Point position(creature_info.x, creature_info.y);
    const auto category = static_cast<SpriteCategory>(creature_info.creature);

    SpriteLayer creature = create_sprite_layer(category, creature_info.variation);
    const SDL2pp::Point size = creature.frame.GetSize();

    EnemySprite sprite(std::move(creature), position, creature_info.direction, size);
    return sprite;
}

FixedSprite SpriteCreator::create_sprite(const LootInfoDTO& loot_info) {
    const SDL2pp::Point position(loot_info.x, loot_info.y);
    SpriteLayer base = create_sprite_layer(SpriteCategory::LOOT,
                                           static_cast<uint8_t>(loot_info.is_item));  // false: 0, true: 1
    const SDL2pp::Point size = base.frame.GetSize();

    FixedSprite loot(std::move(base), position, size);
    return loot;
}

EffectSprite SpriteCreator::create_sprite(const ActionDTO& action_info, SDL2pp::Point position) {
    // TODO ir agregando efectos segun actions
    switch (action_info.action) {
        case ActionType::ATTACK: {
            position = SDL2pp::Point(action_info.attack.x, action_info.attack.y);
            SpriteLayer fx = create_sprite_layer(SpriteCategory::ATTACK_VFX, action_info.attack.weapon);

            EffectSprite effect(std::move(fx), position, fx.frame.GetSize());
            return effect;
        }
        case ActionType::DEATH:
        default: {
            SpriteLayer fx =
                    create_sprite_layer(SpriteCategory::ACTION_VFX, static_cast<uint8_t>(EffectType::DEATH));

            EffectSprite effect(std::move(fx), position, fx.frame.GetSize());
            return effect;
        }
    }
}

FixedSprite SpriteCreator::create_sprite(const SpriteCategory category, const AssetInfoDTO& asset_info) {
    const SDL2pp::Point position(asset_info.x, asset_info.y);
    SpriteLayer base = create_sprite_layer(category, asset_info.id);
    const SDL2pp::Point size = base.frame.GetSize();

    FixedSprite asset(std::move(base), position, size);
    return asset;
}

void SpriteCreator::update_appearance(PlayerSprite& player, const AppearanceDTO& appearance,
                                      const EquipmentInfoDTO& equipment) {
    // En caso de ser fantasma, no se le aplica ningún update
    if (!player.layer_is_different(Layer::HEAD, GHOST_HEAD_ID)) {
        return;
    }

    update_layer(player, SpriteCategory::BODY, Layer::BODY, appearance.body);
    update_layer(player, SpriteCategory::EQUIPMENT, Layer::HELMET, equipment.helmet);
    update_layer(player, SpriteCategory::EQUIPMENT, Layer::SHIELD, equipment.shield);
    update_layer(player, SpriteCategory::EQUIPMENT, Layer::WEAPON, equipment.weapon);
}

void SpriteCreator::update_appearance(PlayerSprite& player, const AppearanceDTO& appearance) {
    update_layer(player, SpriteCategory::BODY, Layer::BODY, appearance.body);
    update_layer(player, SpriteCategory::HEAD, Layer::HEAD, appearance.head);
}

void SpriteCreator::convert_to_ghost(PlayerSprite& player) {
    const AppearanceDTO ghost_appearance = {GHOST_BODY_ID, GHOST_HEAD_ID};
    update_appearance(player, ghost_appearance);
}


SpriteLayer SpriteCreator::create_sprite_layer(const SpriteCategory category, const uint8_t id,
                                               const SDL2pp::Point& offset) {
    SDL2pp::Texture& texture = texture_pool.get_sprite_texture(category, id);

    switch (category) {
        case SpriteCategory::NPC:
        case SpriteCategory::TILE:
        case SpriteCategory::COLLIDER:
        case SpriteCategory::LOOT:
        case SpriteCategory::ATTACK_VFX:
        case SpriteCategory::ACTION_VFX: {
            const Animation action = animation_pool.get_item_animation(category, id);
            return SpriteLayer(renderer, texture, id, offset, action);
        }
        default: {
            auto actions = animation_pool.get_walking_animations(category);
            return SpriteLayer(renderer, texture, id, offset, actions);
        }
    }
}

void SpriteCreator::update_layer(PlayerSprite& player, const SpriteCategory category, const Layer layer,
                                 const uint8_t id) {
    if (!player.layer_is_different(layer, id)) {
        return;
    }
    const auto offset = get_layer_offset(layer);

    SpriteLayer new_layer = create_sprite_layer(category, id, offset);
    new_layer.update_frame(0, player.get_last_direction());  // Apunte a dirección correcta

    player.remove_layer(layer);
    player.add_layer(layer, std::move(new_layer));
}

SDL2pp::Point SpriteCreator::get_layer_offset(const Layer layer) {
    switch (layer) {
        case Layer::HEAD:
        case Layer::HELMET:
            return {0, 0};
        default:
            return {0, HEAD_OFFSET};
    }
}
