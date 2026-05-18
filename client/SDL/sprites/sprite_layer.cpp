#include "sprite_layer.h"
#include <cassert>


SpriteLayer::SpriteLayer(SDL2pp::Renderer &renderer, SDL2pp::Texture& texture, const SDL2pp::Point &offset, const SDL2pp::Rect &frame,
                         std::map<Direction, Animation> &animations) :
    offset(offset), frame(frame), last_action(DOWN), texture(texture), animations(animations), renderer(renderer) {}

void SpriteLayer::render(const SDL2pp::Point &base_position) {
    renderer.Copy(texture, frame, SDL2pp::Rect(base_position+offset,frame.GetSize()));
}

void SpriteLayer::update_frame(const int iteration, const Direction action) {
    assert(action != IDLE);

    const Animation animation = animations.at(action);
    frame = animation.next_frame(iteration);
    last_action = action;
}

void SpriteLayer::set_base_frame() {
    const Animation animation = animations.at(last_action);
    frame = animation.get_first();
}

