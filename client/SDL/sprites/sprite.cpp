#include "sprite.h"

#include "client/client_constants.h"
#include "client/config/client_config.h"


Sprite::Sprite(const SDL2pp::Point position, const SDL2pp::Point size, const SDL2pp::Point render_offset):
        position(to_sprite_point(position)), size(size), render_offset(render_offset) {}

SDL2pp::Point Sprite::get_size() const { return size; }

SDL2pp::Point Sprite::get_position() const { return position; }

SDL2pp::Point Sprite::get_ground_position() const { return position + size - render_offset; }

SDL2pp::Point Sprite::to_sprite_point(const SDL2pp::Point& point) {
    return point * ClientConfig::get().get_tile_size();
}

void Sprite::set_visual_position(const SDL2pp::Point& new_visual_position) { position = new_visual_position; }
