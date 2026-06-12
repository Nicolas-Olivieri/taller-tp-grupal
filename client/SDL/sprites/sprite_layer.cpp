#include "sprite_layer.h"

SpriteLayer::SpriteLayer(SDL2pp::Renderer& renderer, SDL2pp::Texture& texture, const uint8_t id, const SDL2pp::Point& offset,
                         Animation animation):
        offset(offset),
        last_action(std::nullopt),
        id(id),
        texture(texture),
        animations(animation),
        renderer(renderer) {
    set_base_frame();
}


SpriteLayer::SpriteLayer(SDL2pp::Renderer& renderer, SDL2pp::Texture& texture, const uint8_t id, const SDL2pp::Point& offset,
                         std::map<Direction, Animation>& animations):
        offset(offset),
        last_action(Direction::DOWN),
        id(id),
        texture(texture),
        animations(animations),
        renderer(renderer) {
    set_base_frame();
}

void SpriteLayer::render(const SDL2pp::Point& base_position) {
    renderer.Copy(texture, frame, SDL2pp::Rect(base_position + offset, frame.GetSize()));
}

void SpriteLayer::update_frame(const int iteration, const std::optional<Direction> action) {
    // En caso de ser un item con una unica animación
    if (std::holds_alternative<Animation>(animations)) {
        const Animation& anim = std::get<Animation>(animations);
        frame = anim.next_frame(iteration);
    }

    // En caso de ser un sprite con movimiento que tiene Direction
    if (action.has_value() && action.value() != Direction::IDLE) {
        const auto& anim_map = std::get<std::map<Direction, Animation>>(animations);
        frame = anim_map.at(action.value()).next_frame(iteration);
        last_action = action.value();
    }
}

void SpriteLayer::set_base_frame() {
    if (std::holds_alternative<Animation>(animations)) {
        frame = std::get<Animation>(animations).get_first();
    } else {
        const auto& anim_map = std::get<std::map<Direction, Animation>>(animations);
        frame = anim_map.at(last_action.value()).get_first();
    }
}

bool SpriteLayer::has_static_animation() const { return std::holds_alternative<Animation>(animations); }

bool SpriteLayer::texture_is_different(const int other) const {
    return other != id;
}

std::optional<Direction> SpriteLayer::get_last_action() const { return last_action; }
