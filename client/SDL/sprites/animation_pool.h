#ifndef ANIMATION_POOL_H
#define ANIMATION_POOL_H

#include <map>
#include <string>

#include "../../../common/direction.h"

#include "animation.h"

class AnimationPool {
private:
    std::map<std::string, std::map<Direction, Animation>> animations;

public:
    AnimationPool();

    std::map<Direction, Animation>& get_animation(const std::string& category);
};


#endif  // ANIMATION_POOL_H
