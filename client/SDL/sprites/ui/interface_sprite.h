#ifndef INTERFACE_SPRITE_H
#define INTERFACE_SPRITE_H
#include "client/SDL/sprites/base/sprite.h"
#include "client/SDL/sprites/base/sprite_layer.h"


class InterfaceSprite: public Sprite {
private:
    SpriteLayer image;

public:
    InterfaceSprite(SpriteLayer&& image, SDL2pp::Point position, SDL2pp::Point size);

    void render();
};


#endif  // INTERFACE_SPRITE_H
