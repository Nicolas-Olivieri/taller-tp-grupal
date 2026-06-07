#ifndef ANIMATION_POOL_H
#define ANIMATION_POOL_H

#include <map>
#include <string>

#include "../../../common/direction.h"
#include "client/client_constants.h"

#include "animation.h"

class AnimationPool {
private:
    std::map<SpriteCategory, std::map<Direction, Animation>> walking_animations;
    std::map<SpriteCategory, std::map<uint8_t, Animation>> item_animations;

public:
    AnimationPool();

    std::map<Direction, Animation>& get_walking_animations(SpriteCategory category);

    Animation& get_item_animation(SpriteCategory category, uint8_t asset_id);
};


#endif  // ANIMATION_POOL_H
