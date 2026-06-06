#ifndef SPRITE_LAYER_H
#define SPRITE_LAYER_H

#include <map>
#include <variant>

#include "../../../common/direction.h"
#include "SDL2pp/Point.hh"
#include "SDL2pp/Rect.hh"
#include "SDL2pp/Texture.hh"

#include "animation.h"


class SpriteLayer {
    friend class Sprite;
    friend class SpriteCreator;

private:
    SDL2pp::Point offset;
    SDL2pp::Rect frame;
    std::optional<Direction> last_action;

    SDL2pp::Texture& texture;
    std::variant<Animation, std::map<Direction, Animation>> animations;
    SDL2pp::Renderer& renderer;

public:
    SpriteLayer(SDL2pp::Renderer& renderer, SDL2pp::Texture& texture, const SDL2pp::Point& offset,
                Animation animation);

    SpriteLayer(SDL2pp::Renderer& renderer, SDL2pp::Texture& texture, const SDL2pp::Point& offset,
                std::map<Direction, Animation>& animations);

    void render(const SDL2pp::Point& base_position);

    void update_frame(int iteration, std::optional<Direction> action = std::nullopt);

    void set_base_frame();

    bool has_static_animation() const;
};


#endif  // SPRITE_LAYER_H
