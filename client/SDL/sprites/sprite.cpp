#include "sprite.h"

#include <algorithm>

#define PIXELS_PER_STEP 5

Sprite::Sprite(SpriteLayer&& body, const SDL2pp::Point position, const Direction action):
        position(position), direction(action), layers{{Layer::BODY, body}} {}

void Sprite::add_layer(Layer layer_num, SpriteLayer&& layer) { layers.emplace(layer_num, layer); }

void Sprite::remove_layer(const Layer layer_num) { layers.erase(layer_num); }


void Sprite::set_target_position(const Direction new_direction, const SDL2pp::Point& new_target) {
    target_position = new_target;

    if (new_direction != Direction::IDLE) {
        direction = new_direction;
    }
}

void Sprite::update_visual_position() {
    if (position == target_position) {
        direction = Direction::IDLE;
        return;
    }

    const SDL2pp::Point diff = target_position - position;

    switch (direction) {
        case Direction::DOWN:
            position.y += std::min(PIXELS_PER_STEP, diff.y);
            break;
        case Direction::UP:
            position.y -= std::min(PIXELS_PER_STEP, std::abs(diff.y));
            break;
        case Direction::LEFT:
            position.x -= std::min(PIXELS_PER_STEP, std::abs(diff.x));
            break;
        case Direction::RIGHT:
            position.x += std::min(PIXELS_PER_STEP, diff.x);
            break;
        case Direction::IDLE:
        default:
            break;
    }
}

void Sprite::update_frame(const int iteration) {
    for (auto& [_, layer]: layers) {
        if (direction != Direction::IDLE) {
            layer.update_frame(iteration, direction);
        } else {
            layer.set_base_frame();
        }
    }
}

void Sprite::render() {
    if (direction == Direction::UP) {
        for (auto layer = layers.rbegin(); layer != layers.rend(); ++layer) {
            layer->second.render(position);
        }
        return;
    }

    for (auto& layer: layers) {
        layer.second.render(position);
    }
}

SDL2pp::Point Sprite::get_position() const { return position; }

bool Sprite::is_idle() const { return direction == Direction::IDLE; }
