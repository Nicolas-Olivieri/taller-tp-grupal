#include "sprite.h"

#include "client/client_constants.h"
#include <algorithm>
#include <utility>


Sprite::Sprite(const SDL2pp::Point position, const SDL2pp::Point size, const SDL2pp::Point render_offset):
        position(to_sprite_point(position)), size(size), render_offset(render_offset) {}

SDL2pp::Point Sprite::get_size() const { return size; }

SDL2pp::Point Sprite::get_position() const { return position; }

SDL2pp::Point Sprite::get_ground_position() const { return position + size - render_offset; }

SDL2pp::Point Sprite::to_sprite_point(const SDL2pp::Point& point) { return point * TILE_SIZE; }

bool Sprite::has_label() const { return label != nullptr; }

SpriteLabel& Sprite::get_label() const { return *label.get(); }

void Sprite::set_label(std::unique_ptr<SpriteLabel> new_label) { label = std::move(new_label); }

void Sprite::render_overlay(const SDL2pp::Point& camera_offset) const {
    if (not label)
        return;

    label->render(position - camera_offset - render_offset, size);
}
