#include "sprite.h"

#include <algorithm>

#define MIN_PIXELS_PER_STEP 3
#define CHANGE_RATE 0.3

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

    position.x = get_new_coordinate(position.x, diff.x);
    position.y = get_new_coordinate(position.y, diff.y);
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

int Sprite::get_new_coordinate(const int& current_coordinate, const int& coordinate_diff) {
    const int coordinate_movement = coordinate_diff * CHANGE_RATE;

    if (MIN_PIXELS_PER_STEP > std::abs(coordinate_movement)) {
        return current_coordinate + coordinate_diff;  // target coordinate
    } else {
        return current_coordinate + coordinate_movement;
    }
}
