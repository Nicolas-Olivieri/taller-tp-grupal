#ifndef ANIMATION_POOL_H
#define ANIMATION_POOL_H

#include <map>

#include "animation.h"
#include "../../../common/direction.h"

class AnimationPool {
private:
    std::map<uint8_t, std::map<Direction, Animation>> animations;

public:
    AnimationPool();

    std::map<Direction, Animation>& get_animation(uint8_t id);
};


#endif //ANIMATION_POOL_H
