#ifndef SPRITE_LAYER_H
#define SPRITE_LAYER_H

#include <map>
#include <variant>

#include "SDL2pp/Point.hh"
#include "SDL2pp/Rect.hh"
#include "SDL2pp/Texture.hh"
#include "client/SDL/sprite_creation/animation.h"
#include "common/direction.h"

class SpriteLayer {
    friend class SpriteCreator;

private:
    SDL2pp::Point offset;
    SDL2pp::Rect frame;
    std::optional<Direction> last_action;
    uint8_t id;

    SDL2pp::Texture& texture;
    std::variant<Animation, std::map<Direction, Animation>> animations;
    SDL2pp::Renderer& renderer;

public:
    SpriteLayer(SDL2pp::Renderer& renderer, SDL2pp::Texture& texture, uint8_t id, const SDL2pp::Point& offset,
                Animation animation);

    SpriteLayer(SDL2pp::Renderer& renderer, SDL2pp::Texture& texture, uint8_t id, const SDL2pp::Point& offset,
                std::map<Direction, Animation>& animations);

    void render(const SDL2pp::Point& base_position);

    void render(const SDL2pp::Rect& source, const SDL2pp::Rect& dest);

    void update_frame(int iteration, Direction action);

    void update_frame(int iteration);

    void set_base_frame();

    bool has_static_animation() const;

    int get_animation_frame_amount() const;

    uint8_t get_id() const;

    bool is_current_texture(int other) const;

    std::optional<Direction> get_last_action() const;

    SDL2pp::Rect get_frame_area() const;
};


#endif  // SPRITE_LAYER_H
