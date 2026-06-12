#include "player_sprite.h"

PlayerSprite::PlayerSprite(SpriteLayer &&head, SpriteLayer &&body, const SDL2pp::Point position, const SDL2pp::Point size, const Direction direction) :
    MovingSprite(position, size, direction), 
    layers({{Layer::HEAD, std::move(head)}, {Layer::BODY, std::move(body)}}) {}

// METODOS HEREDADOS ::::::::::::::::::

void PlayerSprite::update_frame(const int iteration) {
    for (auto& [_, layer]: layers) {
        if (direction != Direction::IDLE) {
            layer.update_frame(iteration, direction);
        } else {
            layer.set_base_frame();
        }
    }
}

void PlayerSprite::render(const SDL2pp::Point& camera_offset) {
    const SDL2pp::Point render_position = position - camera_offset - render_offset;

    if (direction == Direction::UP || this->get_last_direction() == Direction::UP) {
        for (auto layer = layers.rbegin(); layer != layers.rend(); ++layer) {
            layer->second.render(render_position);
        }
        // Vuelvo a renderizar el casco para que se vea por encima de lo demás
        if (layers.contains(Layer::HELMET)) {
            layers.at(Layer::HELMET).render(render_position);
        }
        return;
    }

    for (auto& layer: layers) {
        layer.second.render(render_position);
    }
}

bool PlayerSprite::intersects(const SDL2pp::Rect& area, const SDL2pp::Point& offset) const {
    for (auto& [_, layer]: layers) {
        const SDL2pp::Rect camera_view(area.GetTopLeft() - offset, area.GetSize());
        if (layer.get_frame_area().Intersects(camera_view)) {
            return true;
        }
    }
    return false;
}

// METODOS PUBLICOS ::::::::::::::::::

void PlayerSprite::add_layer(Layer layer_num, SpriteLayer&& layer) {
    layer.set_base_frame();
    layers.emplace(layer_num, layer);
}

void PlayerSprite::remove_layer(const Layer layer_num) {
    layers.erase(layer_num);
}

void PlayerSprite::remove_all_layers() {
    layers.clear();
}

bool PlayerSprite::layer_is_different(const Layer layer, const int id) const {
    return (!layers.contains(layer) && id != 0) || (layers.contains(layer) && layers.at(layer).texture_is_different(id));
}

Direction PlayerSprite::get_last_direction() const {
    return layers.at(Layer::HEAD).get_last_action().value();
}
