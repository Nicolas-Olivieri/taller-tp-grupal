#include "animation_pool.h"

#define BODY_OFFX 256
#define BODY_OFFY 0
#define BODY_W 27
#define BODY_H 47

#define HEAD_OFFX 162
#define HEAD_OFFY 256
#define HEAD_W 27
#define HEAD_H 64

AnimationPool::AnimationPool() {

    Animation body_down(6, BODY_OFFX, BODY_OFFY, BODY_W, BODY_H);
    Animation body_up(6, BODY_OFFX, BODY_OFFY + BODY_H, BODY_W, BODY_H);
    Animation body_left(5, BODY_OFFX, BODY_OFFY + BODY_H * 2, BODY_W, BODY_H);
    Animation body_right(5, BODY_OFFX, BODY_OFFY + BODY_H * 3, BODY_W, BODY_H);

    Animation head_down(1, HEAD_OFFX, HEAD_OFFY, HEAD_W, HEAD_H);
    Animation head_up(1, HEAD_OFFX, HEAD_OFFY + HEAD_H, HEAD_W, HEAD_H);
    Animation head_left(1, HEAD_OFFX, HEAD_OFFY + HEAD_H * 2, HEAD_W, HEAD_H);
    Animation head_right(1, HEAD_OFFX, HEAD_OFFY + HEAD_H * 3, HEAD_W, HEAD_H);

    std::map<Direction, Animation> bodies;
    bodies.insert({{Direction::DOWN, body_down},
                   {Direction::UP, body_up},
                   {Direction::LEFT, body_left},
                   {Direction::RIGHT, body_right}});

    std::map<Direction, Animation> heads;
    heads.insert({{Direction::DOWN, head_down},
                  {Direction::UP, head_up},
                  {Direction::LEFT, head_left},
                  {Direction::RIGHT, head_right}});

    animations.insert({{1, heads}, {2, bodies}});
}

std::map<Direction, Animation>& AnimationPool::get_animation(const uint8_t id) {
    return animations.at(id);
}
