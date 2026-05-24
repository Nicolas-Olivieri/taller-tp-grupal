#include "sprite.h"

#include <algorithm>

#define PIXELS_PER_STEP 5

Sprite::Sprite(SpriteLayer&& body, const SDL2pp::Point position, const Direction action,
               const SDL2pp::Point size):
        position(position), direction(action), size(size), layers{{Layer::BODY, body}} {}

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

void Sprite::render(const SDL2pp::Point& camera_offset) {
    SDL2pp::Point render_position = position - camera_offset;

    if (direction == Direction::UP) {
        for (auto layer = layers.rbegin(); layer != layers.rend(); ++layer) {
            layer->second.render(render_position);
        }
        return;
    }

    for (auto& layer: layers) {
        layer.second.render(render_position);
    }
}

SDL2pp::Point Sprite::get_position() const { return position; }

SDL2pp::Point Sprite::get_size() const { return size; }

bool Sprite::is_idle() const { return direction == Direction::IDLE; }

bool Sprite::intersects(const SDL2pp::Rect& area, const SDL2pp::Point& offset) const {
    for (auto& [_, layer]: layers) {
        const SDL2pp::Rect offseted_camera(area.GetTopLeft() - offset, area.GetSize());
        if (layer.frame.Intersects(offseted_camera)) {
            return true;
        }
    }
    std::cout << "no intersecte :(" << std::endl;
    return false;
}
