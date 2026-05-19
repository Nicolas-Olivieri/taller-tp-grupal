#ifndef SPRITE_LAYER_H
#define SPRITE_LAYER_H

#include <map>

#include "../../../common/direction.h"
#include "SDL2pp/Point.hh"
#include "SDL2pp/Rect.hh"
#include "SDL2pp/Texture.hh"

#include "animation.h"


class SpriteLayer {
private:
    SDL2pp::Point offset;
    SDL2pp::Rect frame;
    Direction last_action;

    SDL2pp::Texture& texture;
    std::map<Direction, Animation>& animations;
    SDL2pp::Renderer& renderer;

public:
    SpriteLayer(SDL2pp::Renderer& renderer, SDL2pp::Texture& texture, const SDL2pp::Point& offset,
                std::map<Direction, Animation>& animations);

    void render(const SDL2pp::Point& base_position);

    void update_frame(int iteration, Direction action);

    void set_base_frame();
};


#endif  // SPRITE_LAYER_H
