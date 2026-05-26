#include "sprite_layer.h"

#include <cassert>


SpriteLayer::SpriteLayer(SDL2pp::Renderer& renderer, SDL2pp::Texture& texture, const SDL2pp::Point& offset,
                         std::map<Direction, Animation>& animations):
        offset(offset),
        last_action(Direction::DOWN),
        texture(texture),
        animations(animations),
        renderer(renderer) {
    set_base_frame();
}

void SpriteLayer::render(const SDL2pp::Point& base_position) {
    renderer.Copy(texture, frame, SDL2pp::Rect(base_position + offset, frame.GetSize()));
}

void SpriteLayer::update_frame(const int iteration, const Direction action) {
    assert(action != Direction::IDLE);

    const Animation animation = animations.at(action);
    frame = animation.next_frame(iteration);
    last_action = action;
}

void SpriteLayer::set_base_frame() {
    const Animation animation = animations.at(last_action);
    frame = animation.get_first();
}
