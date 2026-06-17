#include "world_sprite.h"

#include "client/client_constants.h"

WorldSprite::WorldSprite(const SDL2pp::Point position, const SDL2pp::Point size,
                         const SDL2pp::Point render_offset):
        Sprite(to_sprite_point(position), size, render_offset) {}


SDL2pp::Point WorldSprite::get_ground_position() const { return position + size - render_offset; }

SDL2pp::Point WorldSprite::to_sprite_point(const SDL2pp::Point& point) { return point * TILE_SIZE; }
