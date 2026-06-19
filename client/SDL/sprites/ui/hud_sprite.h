#ifndef HUD_SPRITE_H
#define HUD_SPRITE_H
#include <memory>

#include "client/SDL/sprites/base/sprite_layer.h"

#include "text_sprite.h"


class HudSprite: public Sprite {
private:
    SDL2pp::Renderer& renderer;

    std::unique_ptr<SpriteLayer> image;
    SDL2pp::Rect dest;
    std::optional<TextSprite> amount_label;
    bool bounded;

public:
    HudSprite(SDL2pp::Renderer& renderer, std::unique_ptr<SpriteLayer>&& image, const SDL2pp::Point& position,
              const SDL2pp::Point& size, const SDL2pp::Rect& dest);

    HudSprite(SDL2pp::Renderer& renderer, std::unique_ptr<SpriteLayer>&& image, TextSprite&& amount_label,
              const SDL2pp::Point& position, const SDL2pp::Point& size, const SDL2pp::Rect& dest);

    void render();

    void bind();
    void unbind();

    void update_appearance(std::unique_ptr<SpriteLayer>&& new_image);
    void update_amount_label(int new_amount);

    uint8_t get_id() const;
    bool is_current_appearance(uint8_t id) const;
};


#endif  // HUD_SPRITE_H
