#ifndef SPRITE_CREATOR_H
#define SPRITE_CREATOR_H

#include <memory>

#include "../sprites/sprite_layer.h"
#include "client/SDL/sprites/effect_sprite.h"
#include "client/SDL/sprites/enemy_sprite.h"
#include "client/SDL/sprites/fixed_sprite.h"
#include "client/SDL/sprites/player_sprite.h"
#include "common/dto/snapshot/actions/action.h"
#include "common/dto/snapshot/info/appearance.h"
#include "common/dto/snapshot/info/creatureinfo.h"
#include "common/dto/snapshot/info/lootinfo.h"
#include "common/dto/snapshot/info/playerinfo.h"
#include "common/dto/snapshot/map/asset_info.h"

#include "animation_pool.h"
#include "texture_pool.h"


class SpriteCreator {
private:
    TexturePool texture_pool;
    AnimationPool animation_pool;
    SDL2pp::Renderer& renderer;
    FontManager& font_manager;

    SpriteLayer create_sprite_layer(SpriteCategory category, uint8_t id,
                                    const SDL2pp::Point& offset = SDL2pp::Point(0, 0));

    void update_layer(PlayerSprite& player, SpriteCategory category, Layer layer, uint8_t id);

    SDL2pp::Point get_layer_offset(Layer layer);

public:
    explicit SpriteCreator(SDL2pp::Renderer& renderer, FontManager& font_manager);

    // Sobrecarga para tomar distintos DTOs

    PlayerSprite create_sprite(const PlayerInfoDTO& player_info);

    FixedSprite create_sprite(SpriteCategory category, const AssetInfoDTO& asset_info);

    EffectSprite create_sprite(const ActionDTO& action_info, SDL2pp::Point position = {0, 0});

    EnemySprite create_sprite(const CreatureInfoDTO& creature_info);

    FixedSprite create_sprite(const LootInfoDTO& loot_info);

    void update_appearance(PlayerSprite& player, const AppearanceDTO& appearance,
                           const EquipmentInfoDTO& equipment);

    void update_appearance(PlayerSprite& player, const AppearanceDTO& appearance);

    void convert_to_ghost(PlayerSprite& player);

    template <typename InfoDTO>
    void update_label(MovingSprite& sprite, const InfoDTO& info) const {
        if (not sprite.has_label()) {
            sprite.set_label(std::make_unique<SpriteLabel>(renderer, font_manager, info));
        } else {
            sprite.get_label().update(info);
        }
    }
};


#endif  // SPRITE_CREATOR_H
