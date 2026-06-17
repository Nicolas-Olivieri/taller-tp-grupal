#ifndef PROGRESS_BAR_SPRITE_H
#define PROGRESS_BAR_SPRITE_H
#include "text_sprite.h"
#include "client/SDL/sprites/base/sprite_layer.h"


class ProgressBarSprite : public Sprite {
private:
    SpriteLayer image;
    SDL2pp::Rect box;
    size_t max;
    size_t current;

    std::optional<TextSprite> label;

public:
    ProgressBarSprite(SpriteLayer &&image, const SDL2pp::Point &position, const SDL2pp::Point &size, SDL2pp::Rect box,
                      size_t current, size_t max);

    ProgressBarSprite(SpriteLayer &&image, TextSprite &&label, const SDL2pp::Point &position, const SDL2pp::Point &size,
                      SDL2pp::Rect box, size_t current, size_t max);

    void render();

    void update_values(size_t new_current, size_t new_max);
};


#endif //PROGRESS_BAR_SPRITE_H
