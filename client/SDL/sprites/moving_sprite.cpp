#include "moving_sprite.h"

#include <memory>
#include <utility>

#include "client/client_constants.h"

#define MIN_PIXELS_PER_STEP 3
#define CHANGE_RATE 0.3

MovingSprite::MovingSprite(const SDL2pp::Point position, const SDL2pp::Point size, const Direction direction):
        Sprite(position, size, SDL2pp::Point{(size.x - TILE_SIZE) / 2, size.y - TILE_SIZE}),
        direction(direction) {}

// METODOS PUBLICOS ::::::::::::::::::

void MovingSprite::set_target_position(const Direction new_direction, const SDL2pp::Point& new_target) {
    target_position = to_sprite_point(new_target);

    if (new_direction != Direction::IDLE) {
        direction = new_direction;
    }
}

void MovingSprite::update_visual_position() {
    if (position == target_position) {
        direction = Direction::IDLE;
        return;
    }
    const SDL2pp::Point diff = target_position - position;
    position.x = get_new_coordinate(position.x, diff.x);
    position.y = get_new_coordinate(position.y, diff.y);
}

SDL2pp::Point MovingSprite::get_target_position() const { return target_position; }

Direction MovingSprite::get_direction() const { return direction; }

bool MovingSprite::is_idle() const { return direction == Direction::IDLE; }

bool MovingSprite::has_label() const { return label != nullptr; }

SpriteLabel& MovingSprite::get_label() const { return *label.get(); }

void MovingSprite::set_label(std::unique_ptr<SpriteLabel> new_label) { label = std::move(new_label); }

void MovingSprite::render_overlay(const SDL2pp::Point& camera_offset) const {
    if (not label)
        return;

    label->render(position - camera_offset - render_offset, size);
}

// METODOS PRIVADOS ::::::::::::::::::

int MovingSprite::get_new_coordinate(const int& current_coordinate, const int& coordinate_diff) {
    const int coordinate_movement = coordinate_diff * CHANGE_RATE;

    if (MIN_PIXELS_PER_STEP > std::abs(coordinate_movement)) {
        return current_coordinate + coordinate_diff;  // target coordinate
    } else {
        return current_coordinate + coordinate_movement;
    }
}
