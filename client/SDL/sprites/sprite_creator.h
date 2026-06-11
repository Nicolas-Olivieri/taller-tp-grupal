#ifndef SPRITE_CREATOR_H
#define SPRITE_CREATOR_H

#include <string>

#include "common/dto/snapshot/info/appearance.h"
#include "common/dto/snapshot/info/creatureinfo.h"
#include "common/dto/snapshot/info/lootinfo.h"
#include "common/dto/snapshot/info/playerinfo.h"
#include "common/dto/snapshot/map/asset_info.h"

#include "animation_pool.h"
#include "sprite.h"
#include "sprite_layer.h"
#include "texture_pool.h"


class SpriteCreator {
private:
    TexturePool texture_pool;
    AnimationPool animation_pool;
    SDL2pp::Renderer& renderer;

    SpriteLayer create_sprite_layer(SpriteCategory category, uint8_t id,
                                    const SDL2pp::Point& offset = SDL2pp::Point(0, 0));

    void update_layer(Sprite &player, SpriteCategory category, Layer layer, uint8_t id);

public:
    explicit SpriteCreator(SDL2pp::Renderer& renderer);

    // Sobrecarga para tomar distintos DTOs

    Sprite create_sprite(const PlayerInfoDTO& player_info);

    Sprite create_sprite(SpriteCategory category, const AssetInfoDTO& asset_info);

    Sprite create_sprite(const CreatureInfoDTO& creature_info);

    Sprite create_sprite(const LootInfoDTO& loot_info);

    void update_appearance(Sprite &player, const AppearanceDTO& appearance, const EquipmentInfoDTO &equipment);

    void update_appearance(Sprite &player, const AppearanceDTO &appearance);

    void convert_to_ghost(Sprite& player);
};


#endif  // SPRITE_CREATOR_H
