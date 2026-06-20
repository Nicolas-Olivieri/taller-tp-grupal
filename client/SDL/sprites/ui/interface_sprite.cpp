#include "interface_sprite.h"

#include <utility>

InterfaceSprite::InterfaceSprite(SpriteLayer&& image, const SDL2pp::Point position, const SDL2pp::Point size,
                                 const SDL2pp::Rect dest):
        Sprite(position, size, {0, 0}), dest(dest), image(std::move(image)) {}

void InterfaceSprite::render() { image.render(image.get_frame_area(), dest); }
