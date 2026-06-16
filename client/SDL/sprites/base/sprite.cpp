#include "sprite.h"


Sprite::Sprite(const SDL2pp::Point position, const SDL2pp::Point size, const SDL2pp::Point render_offset):
        position(position), size(size), render_offset(render_offset) {}

SDL2pp::Point Sprite::get_size() const { return size; }

SDL2pp::Point Sprite::get_position() const { return position; }

void Sprite::set_visual_position(const SDL2pp::Point& new_visual_position) { position = new_visual_position; }
