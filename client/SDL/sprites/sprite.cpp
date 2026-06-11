#include "sprite.h"

#include <algorithm>

#define MIN_PIXELS_PER_STEP 3
#define CHANGE_RATE 0.3

Sprite::Sprite(SpriteLayer&& base, const SDL2pp::Point position, const SDL2pp::Point size):
        position(to_sprite_point(position)),
        target_position(this->position),
        direction(std::nullopt),
        size(size),
        layers({{Layer::BODY, base}}) {}

Sprite::Sprite(SpriteLayer&& body, const SDL2pp::Point position, const Direction action,
               const SDL2pp::Point size):
        position(to_sprite_point(position)),
        target_position(this->position),
        direction(action),
        size(size),
        render_offset(SDL2pp::Point{(size.x - TILE_SIZE) / 2, size.y - TILE_SIZE}),
        layers{{Layer::BODY, body}} {}

void Sprite::add_layer(Layer layer_num, SpriteLayer&& layer) { layers.emplace(layer_num, layer); }

void Sprite::remove_layer(const Layer layer_num) { layers.erase(layer_num); }

const SDL2pp::Point& Sprite::get_target_position() const { return target_position; }

void Sprite::set_target_position(const Direction new_direction, const SDL2pp::Point& new_target) {
    target_position = to_sprite_point(new_target);

    if (new_direction != Direction::IDLE) {
        direction = new_direction;
    }
}

void Sprite::update_visual_position() {
    if (!direction.has_value()) {
        return;
    }

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
        // En caso de ser un item con una unica animación, no tiene direction
        if (!direction.has_value()) {
            layer.update_frame(iteration);
            continue;
        }

        // En caso de ser un sprite que se mueve, tendrá direction
        if (direction.value() != Direction::IDLE) {
            layer.update_frame(iteration, direction.value());
        } else {
            layer.set_base_frame();
        }
    }
}

void Sprite::render(const SDL2pp::Point& camera_offset) {
    const SDL2pp::Point render_position = position - camera_offset - render_offset;

    if (direction == Direction::UP || layers.at(Layer::BODY).last_action == Direction::UP) {
        const auto order = {Layer::SHIELD, Layer::WEAPON, Layer::BODY, Layer::HEAD, Layer::HELMET};
        for (auto& layer : order) {
            if (layers.contains(layer)) {
                layers.at(layer).render(render_position);
            }
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

int Sprite::get_new_coordinate(const int& current_coordinate, const int& coordinate_diff) {
    const int coordinate_movement = coordinate_diff * CHANGE_RATE;

    if (MIN_PIXELS_PER_STEP > std::abs(coordinate_movement)) {
        return current_coordinate + coordinate_diff;  // target coordinate
    } else {
        return current_coordinate + coordinate_movement;
    }
}

SDL2pp::Point Sprite::to_sprite_point(const SDL2pp::Point& point) { return point * TILE_SIZE; }

bool Sprite::intersects(const SDL2pp::Rect& area, const SDL2pp::Point& offset) const {
    for (auto& [_, layer]: layers) {
        const SDL2pp::Rect offseted_camera(area.GetTopLeft() - offset, area.GetSize());
        if (layer.frame.Intersects(offseted_camera)) {
            return true;
        }
    }
    return false;
}

bool Sprite::layer_is_different(const Layer layer, const int id) const {
    return (!layers.contains(layer) && id != 0) || (layers.contains(layer) && layers.at(layer).texture_is_different(id));
}

void Sprite::remove_all_layers() { layers.clear(); }

SDL2pp::Point Sprite::get_ground_position() const { return position + size - render_offset; }
