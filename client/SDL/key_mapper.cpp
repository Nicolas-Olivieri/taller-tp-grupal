#include "key_mapper.h"

#include <cassert>


bool KeyMapper::is_movement_key(const int& key_pressed) {
    return key_to_move_map.contains(key_pressed);
}

Direction KeyMapper::get_direction(const int& key_pressed) {
    assert(is_movement_key(key_pressed));

    return key_to_move_map.at(key_pressed);
}
