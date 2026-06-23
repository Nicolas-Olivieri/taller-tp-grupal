#ifndef INTERFACE_SPRITE_H
#define INTERFACE_SPRITE_H
#include "client/SDL/sprites/base/sprite.h"
#include "client/SDL/sprites/base/sprite_layer.h"


class InterfaceSprite final: public Sprite {
private:
    SDL2pp::Rect dest;
    SpriteLayer image;

public:
    InterfaceSprite(SpriteLayer&& image, SDL2pp::Point position, SDL2pp::Point size, SDL2pp::Rect dest);

    void render();
};


#endif  // INTERFACE_SPRITE_H
