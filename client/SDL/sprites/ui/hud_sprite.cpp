#include "hud_sprite.h"

#include <memory>
#include <utility>

HudSprite::HudSprite(SDL2pp::Renderer& renderer, std::unique_ptr<SpriteLayer>&& image,
                     const SDL2pp::Point& position, const SDL2pp::Point& size, const SDL2pp::Rect& dest):
        Sprite(position, size, dest.GetSize() - size),
        renderer(renderer),
        image(std::move(image)),
        dest(dest),
        amount_label(std::nullopt),
        bounded(false) {}


HudSprite::HudSprite(SDL2pp::Renderer& renderer, std::unique_ptr<SpriteLayer>&& image,
                     TextSprite&& amount_label, const SDL2pp::Point& position, const SDL2pp::Point& size,
                     const SDL2pp::Rect& dest):
        Sprite(position, size, dest.GetSize() - size),
        renderer(renderer),
        image(std::move(image)),
        dest(dest),
        amount_label(std::move(amount_label)),
        bounded(false) {}


void HudSprite::update_appearance(std::unique_ptr<SpriteLayer>&& new_image) { image = std::move(new_image); }

void HudSprite::update_amount_label(const int new_amount) {
    if (amount_label) {
        amount_label->set_text(new_amount == 0 ? "" : std::format("{}", new_amount));
    }
}

bool HudSprite::is_current_appearance(const uint8_t id) const { return image->is_current_texture(id); }

void HudSprite::render() {
    image->render(image->get_frame_area(), dest);
    if (amount_label) {
        amount_label->render_left();
    }
    if (bounded) {
        constexpr SDL_Color yellow = {235, 224, 70, 255};
        renderer.SetDrawColor(yellow.r, yellow.g, yellow.b, yellow.a);
        renderer.DrawRect(position, position + dest.GetSize());
        renderer.SetDrawColor(0, 0, 0, 255);
    }
}

void HudSprite::bind() { bounded = true; }

void HudSprite::unbind() { bounded = false; }

uint8_t HudSprite::get_id() const { return image->get_id(); }
