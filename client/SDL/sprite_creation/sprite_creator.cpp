#include "sprite_creator.h"

#include <map>
#include <memory>
#include <ranges>
#include <string>
#include <utility>

#include <qcolor.h>

#include "client/SDL/sprites/base/sprite_label.h"
#include "client/SDL/sprites/fixed/effect_sprite.h"
#include "client/SDL/sprites/fixed/fixed_sprite.h"
#include "client/SDL/sprites/moving/enemy_sprite.h"
#include "client/SDL/sprites/moving/player_sprite.h"
#include "client/SDL/sprites/ui/hud_sprite.h"
#include "client/SDL/sprites/ui/interface_sprite.h"
#include "client/SDL/sprites/ui/progress_bar_sprite.h"
#include "client/SDL/sprites/ui/text_sprite.h"
#include "client/client_constants.h"
#include "client/config/client_config.h"
#include "common/dto/snapshot/actions/action.h"
#include "common/dto/snapshot/map/asset_info.h"

SpriteCreator::SpriteCreator(SDL2pp::Renderer& renderer, FontManager& font_manager):
        texture_pool(TexturePool(renderer)),
        animation_pool(AnimationPool()),
        renderer(renderer),
        font_manager(font_manager) {}


PlayerSprite SpriteCreator::create_sprite(const PlayerInfoDTO& player_info, const bool is_client_player) {
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
    if (!is_client_player) {
        sprite.set_label(std::make_unique<SpriteLabel>(renderer, font_manager, player_info));
    }

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
    sprite.set_label(std::make_unique<SpriteLabel>(renderer, font_manager, creature_info));

    return sprite;
}

FixedSprite SpriteCreator::create_sprite(const LootInfoDTO& loot_info) {
    const SDL2pp::Point position(loot_info.x, loot_info.y);
    SpriteLayer base = create_sprite_layer(SpriteCategory::LOOT, static_cast<uint8_t>(loot_info.type));
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

        case ActionType::MEDITATION:
            SpriteLayer fx = create_sprite_layer(SpriteCategory::ACTION_VFX,
                                                 static_cast<uint8_t>(EffectType::MEDITATION));
            return EffectSprite(std::move(fx), position, fx.frame.GetSize());
    }
}

FixedSprite SpriteCreator::create_sprite(const SpriteCategory category, const AssetInfoDTO& asset_info) {
    const SDL2pp::Point position(asset_info.x, asset_info.y);
    SpriteLayer base = create_sprite_layer(category, asset_info.id);
    const SDL2pp::Point size = base.frame.GetSize();

    FixedSprite asset(std::move(base), position, size);
    return asset;
}

InterfaceSprite SpriteCreator::create_sprite(UiElement ui_type, const SDL2pp::Rect& box) {
    SpriteLayer base = create_sprite_layer(SpriteCategory::UI, static_cast<int>(ui_type));
    SDL2pp::Point size = base.frame.GetSize();

    InterfaceSprite ui(std::move(base), box.GetTopLeft(), size, box);
    return ui;
}

ProgressBarSprite SpriteCreator::create_sprite(UiElement bar_type, const SDL2pp::Rect dest_rect,
                                               size_t current, size_t max) {
    SpriteLayer base = create_sprite_layer(SpriteCategory::UI, static_cast<int>(bar_type));
    SDL2pp::Point size = base.frame.GetSize();

    TextSprite label =
            create_sprite(dest_rect, "", FontType::UI_MENU, ClientConfig::get().get_color_data().white);

    ProgressBarSprite ui(std::move(base), (std::move(label)), dest_rect.GetTopLeft(), size, dest_rect,
                         current, max);
    return ui;
}

HudSprite SpriteCreator::create_sprite(const uint8_t id, const SDL2pp::Rect& dest_rect, bool has_amount) {
    SpriteLayer base = create_sprite_layer(SpriteCategory::HUD, id);
    SDL2pp::Point size = base.frame.GetSize();
    SDL2pp::Rect texture_rect(dest_rect.x, dest_rect.y, size.x, size.y);
    auto ptr = std::make_unique<SpriteLayer>(base);

    if (has_amount) {
        SDL2pp::Rect amount_rect(texture_rect.GetTopLeft() + SDL2pp::Point(4, 0), texture_rect.GetSize());
        TextSprite amount_label = create_sprite(amount_rect, "", FontType::UI_ITEM_AMOUNT,
                                                ClientConfig::get().get_color_data().white);

        HudSprite item(renderer, std::move(ptr), std::move(amount_label), texture_rect.GetTopLeft(), size,
                       dest_rect);
        return item;
    }

    HudSprite item(renderer, std::move(ptr), dest_rect.GetTopLeft(), size, dest_rect);
    return item;
}

TextSprite SpriteCreator::create_sprite(const SDL2pp::Rect box, const std::string& text,
                                        const FontType font_type, const SDL2pp::Color font_color) {
    SDL2pp::Font& font = font_manager.get_font(font_type);
    TextSprite label(renderer, box, text, font, font_color);

    return label;
}


void SpriteCreator::update_appearance(PlayerSprite& player, const AppearanceDTO& appearance,
                                      const EquipmentInfoDTO& equipment) {
    // En caso de ser fantasma, no se le aplica ningún update
    if (!player.layer_is_different(Layer::HEAD, ClientConfig::get().get_ghost_head_id())) {
        return;
    }

    if (equipment.armor.item_id == NO_ITEM) {
        update_layer(player, SpriteCategory::BODY, Layer::BODY, appearance.body);
    } else {
        update_layer(player, SpriteCategory::EQUIPMENT, Layer::BODY, equipment.armor.item_id);
    }
    update_layer(player, SpriteCategory::EQUIPMENT, Layer::HELMET, equipment.helmet.item_id);
    update_layer(player, SpriteCategory::EQUIPMENT, Layer::SHIELD, equipment.shield.item_id);
    update_layer(player, SpriteCategory::EQUIPMENT, Layer::WEAPON, equipment.weapon.item_id);
}

void SpriteCreator::update_appearance(PlayerSprite& player, const AppearanceDTO& appearance) {
    update_layer(player, SpriteCategory::BODY, Layer::BODY, appearance.body);
    update_layer(player, SpriteCategory::HEAD, Layer::HEAD, appearance.head);
}

void SpriteCreator::convert_to_ghost(PlayerSprite& player) {
    const auto& config = ClientConfig::get();
    const AppearanceDTO ghost_appearance = {config.get_ghost_body_id(), config.get_ghost_head_id()};
    update_appearance(player, ghost_appearance);

    if (player.layer_is_different(Layer::HELMET, NO_ITEM))
        player.remove_layer(Layer::HELMET);
    if (player.layer_is_different(Layer::SHIELD, NO_ITEM))
        player.remove_layer(Layer::SHIELD);
    if (player.layer_is_different(Layer::WEAPON, NO_ITEM))
        player.remove_layer(Layer::WEAPON);
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
            return {0, ClientConfig::get().get_head_offset()};
    }
}


void SpriteCreator::update_appearance(HudSprite& item, const uint8_t id, const int amount) {
    item.update_amount_label(amount);
    if (item.is_current_appearance(id)) {
        return;
    }

    SpriteLayer new_appearance = create_sprite_layer(SpriteCategory::HUD, id);
    auto ptr = std::make_unique<SpriteLayer>(new_appearance);
    item.update_appearance(std::move(ptr));
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
        case SpriteCategory::ACTION_VFX:
        case SpriteCategory::UI:
        case SpriteCategory::HUD:
        case SpriteCategory::SAFE_ZONE: {
            const Animation action = animation_pool.get_item_animation(category, id);
            return SpriteLayer(renderer, texture, id, offset, action);
        }
        default: {
            auto actions = animation_pool.get_walking_animations(category);
            return SpriteLayer(renderer, texture, id, offset, actions);
        }
    }
}
