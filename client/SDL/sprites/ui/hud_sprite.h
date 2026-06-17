#ifndef HUD_SPRITE_H
#define HUD_SPRITE_H

#include "text_sprite.h"
#include "../fixed/fixed_sprite.h"


class HudSprite : public Sprite {
private:
    SDL2pp::Renderer &renderer;

    std::unique_ptr<SpriteLayer> image;
    std::optional<TextSprite> amount_label;
    bool bounded;

public:
    HudSprite(SDL2pp::Renderer& renderer, std::unique_ptr<SpriteLayer>&& image, const SDL2pp::Point& position, const SDL2pp::Point& size);

    HudSprite(SDL2pp::Renderer& renderer, std::unique_ptr<SpriteLayer>&& image, TextSprite&& amount_label, const SDL2pp::Point& position, const SDL2pp::Point& size);

    void render();

    void bind();
    void unbind();

    void update_appearance(std::unique_ptr<SpriteLayer> &&new_image);
    void update_amount_label(int new_amount);

    uint8_t get_id() const;
    bool is_current_appearance(uint8_t id) const;
};


#endif //HUD_SPRITE_H
