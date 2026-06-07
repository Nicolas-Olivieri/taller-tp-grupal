#ifndef KEY_MAPPER_H
#define KEY_MAPPER_H


#include <map>

#include "common/direction.h"

#include "SDL_keycode.h"

class KeyMapper {

    inline static const std::map<int, Direction> key_to_move_map = {
            {SDLK_DOWN, Direction::DOWN}, {SDLK_UP, Direction::UP},  {SDLK_RIGHT, Direction::RIGHT},
            {SDLK_LEFT, Direction::LEFT}, {SDLK_s, Direction::DOWN}, {SDLK_w, Direction::UP},
            {SDLK_d, Direction::RIGHT},   {SDLK_a, Direction::LEFT}};

public:
    static bool is_movement_key(const int& key_pressed);

    static Direction get_direction(const int& key_pressed);
};


#endif  // KEY_MAPPER_H
