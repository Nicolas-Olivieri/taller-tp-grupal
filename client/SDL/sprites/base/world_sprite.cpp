#include "world_sprite.h"

#include "client/config/client_config.h"

WorldSprite::WorldSprite(const SDL2pp::Point position, const SDL2pp::Point size,
                         const SDL2pp::Point render_offset):
        Sprite(to_sprite_point(position), size, render_offset), last_frame(-1) {}


SDL2pp::Point WorldSprite::get_ground_position() const { return position + size - render_offset; }

void WorldSprite::set_last_frame(const int iteration) { last_frame = iteration; }

bool WorldSprite::already_selected_for_frame(const int iteration) const { return iteration == last_frame; }

SDL2pp::Point WorldSprite::to_sprite_point(const SDL2pp::Point& point) {
    return point * ClientConfig::get().get_tile_size();
}
