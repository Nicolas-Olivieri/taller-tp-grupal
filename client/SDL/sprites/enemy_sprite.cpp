#include "enemy_sprite.h"

EnemySprite::EnemySprite(SpriteLayer &&body, const SDL2pp::Point position, const Direction direction, const SDL2pp::Point size) :
    MovingSprite(position, size, direction), body(std::move(body)) {}

// METODOS HEREDADOS ::::::::::::::::::

void EnemySprite::update_frame(const int iteration) {
    if (direction != Direction::IDLE) {
        body.update_frame(iteration, direction);
    } else {
        body.set_base_frame();
    }
}

void EnemySprite::render(const SDL2pp::Point& camera_offset) {
    const SDL2pp::Point render_position = position - camera_offset - render_offset;
    body.render(render_position);
}

bool EnemySprite::intersects(const SDL2pp::Rect& area, const SDL2pp::Point& offset) const {
    const SDL2pp::Rect camera_view(area.GetTopLeft() - offset, area.GetSize());

    if (body.get_frame_area().Intersects(camera_view)) {
        return true;
    }
    return false;
}
