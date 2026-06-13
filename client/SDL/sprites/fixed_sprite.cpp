#include "fixed_sprite.h"

#include <utility>

FixedSprite::FixedSprite(SpriteLayer&& image, const SDL2pp::Point& position, const SDL2pp::Point& size):
        Sprite(position, size, SDL2pp::Point(0, 0)), image(std::move(image)) {}

void FixedSprite::update_frame(const int iteration) { image.update_frame(iteration); }

void FixedSprite::render(const SDL2pp::Point& camera_offset) {
    const SDL2pp::Point render_position = position - camera_offset - render_offset;
    image.render(render_position);
}

bool FixedSprite::intersects(const SDL2pp::Rect& area, const SDL2pp::Point& offset) const {
    const SDL2pp::Rect camera_view(area.GetTopLeft() - offset, area.GetSize());

    if (image.get_frame_area().Intersects(camera_view)) {
        return true;
    }
    return false;
}
