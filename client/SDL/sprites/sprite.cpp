#include "sprite.h"

#define PIXELS_PER_STEP 5

Sprite::Sprite(SpriteLayer &&body, const SDL2pp::Point position, const Direction action) :
    position(position), remaining_frames(0), direction(action), layers{{Layer::BODY, body}}
{}

void Sprite::add_layer(Layer layer_num, SpriteLayer &&layer) {
    layers.emplace(layer_num, layer);
}

void Sprite::remove_layer(const Layer layer_num) {
    layers.erase(layer_num);
}

void Sprite::update_position(const Direction new_direction, const SDL2pp::Point &new_position) {
    target_position = new_position;

    if (position == target_position) {
        direction = IDLE;
        return;
    }

    direction = new_direction;
    const SDL2pp::Point diff = target_position - position;
    switch (direction) {
        case DOWN:
            position.y += std::min(PIXELS_PER_STEP, diff.y);
            break;
        case UP:
            position.y -= std::min(PIXELS_PER_STEP, std::abs(diff.y));
            break;
        case LEFT:
            position.x -= std::min(PIXELS_PER_STEP, std::abs(diff.x));
            break;
        case RIGHT:
            position.x += std::min(PIXELS_PER_STEP, diff.x);
            break;
        case IDLE:
            break;
    }
}

void Sprite::update_frame(const int iteration) {
    for (auto&[_, layer] : layers) {
        if (direction != IDLE) {
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

    for (auto& layer : layers) {
        layer.second.render(position);
    }

}

SDL2pp::Point Sprite::get_position() const {return position;}