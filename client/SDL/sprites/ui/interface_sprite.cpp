#include "interface_sprite.h"

InterfaceSprite::InterfaceSprite(SpriteLayer&& image, const SDL2pp::Point position, const SDL2pp::Point size) :
    Sprite(position, size, {0, 0}), image(std::move(image)) {}

void InterfaceSprite::render() {
    image.render(position);
}
