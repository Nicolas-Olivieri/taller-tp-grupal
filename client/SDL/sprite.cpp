#include "sprite.h"

Sprite::Sprite(SDL2pp::Renderer& renderer, SDL2pp::Texture& texture, const int x, const int y, const int width, const int height)
    : position(SDL2pp::Point(x,y)), size(SDL2pp::Point(width*2, height*2)), idle(true),
      current_action(WALK_DOWN), current_frame(SDL2pp::Rect(256, 0, 27, 47)),
      texture(texture), renderer(renderer) {

    Animation walk_down( 5, 256, 0, height, width);
    Animation walk_up( 5, 256, 47, height, width);
    Animation walk_left( 5, 256, 94, height, width);
    Animation walk_right( 5, 256, 141, height, width);

    animations.insert({{WALK_DOWN, walk_down},
                            {WALK_UP, walk_up},
                            {WALK_LEFT, walk_left},
                            {WALK_RIGHT, walk_right}});
}

void Sprite::render() const {
    renderer.Copy(texture, current_frame, SDL2pp::Rect(position, size));
}

void Sprite::update_frame(const int iteration) {
    if (idle) { return; }

    // tal vez haya que resetear la animación si detecta que cambio el action?
    const Animation animation = animations.at(current_action);
    current_frame = animation.next_frame(iteration);
}

void Sprite::update(const Action direction, const SDL2pp::Point& new_position) {
    if (direction != IDLE) {
        idle = false;
        this->current_action = direction;
    }
    if (direction == IDLE) {
        idle = true;
    }
    this->position = new_position;
}

SDL2pp::Point Sprite::get_position() const { return position; }