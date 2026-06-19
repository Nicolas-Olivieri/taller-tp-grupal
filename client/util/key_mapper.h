#ifndef KEY_MAPPER_H
#define KEY_MAPPER_H


#include <map>
#include <unordered_set>

#include "common/direction.h"

#include "SDL_keycode.h"

class KeyMapper {

    inline static const std::map<int, Direction> key_to_move_map = {
            {SDLK_DOWN, Direction::DOWN}, {SDLK_UP, Direction::UP},  {SDLK_RIGHT, Direction::RIGHT},
            {SDLK_LEFT, Direction::LEFT}, {SDLK_s, Direction::DOWN}, {SDLK_w, Direction::UP},
            {SDLK_d, Direction::RIGHT},   {SDLK_a, Direction::LEFT}};

    inline static const std::unordered_set<int> command_keys = {
            {SDLK_q, SDLK_e,
             /* SDLK_m ,*/ SDLK_c}  // TODO: dejo la M comentada para cuando esté lo de meditar
    };

public:
    static bool is_movement_key(const int& key_pressed);

    static bool is_command_key(const int& key_pressed);

    static Direction get_direction(const int& key_pressed);
};


#endif  // KEY_MAPPER_H
