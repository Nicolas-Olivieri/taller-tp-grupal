#include "hud_sprite.h"

HudSprite::HudSprite(SDL2pp::Renderer& renderer, std::unique_ptr<SpriteLayer>&& image, const SDL2pp::Point &position, const SDL2pp::Point &size) :
    Sprite(position, size + SDL2pp::Point{2, 2}, SDL2pp::Point{1, 1} ),
    renderer(renderer), image(std::move(image)), amount_label(std::nullopt), bounded(false) {}


HudSprite::HudSprite(SDL2pp::Renderer& renderer, std::unique_ptr<SpriteLayer> &&image, TextSprite &&amount_label, const SDL2pp::Point &position,
                     const SDL2pp::Point &size) :
    Sprite(position, size + SDL2pp::Point{2, 2}, SDL2pp::Point{1, 1} ),
    renderer(renderer), image(std::move(image)), amount_label(std::move(amount_label)), bounded(false) {}


void HudSprite::update_appearance(std::unique_ptr<SpriteLayer> &&new_image) {
    image = std::move(new_image);
}

void HudSprite::update_amount_label(const int new_amount) {
    if (amount_label) {
        amount_label->set_text(new_amount == 0 ? "" : std::format("{}", new_amount));
    }
}

bool HudSprite::is_current_appearance(const uint8_t id) const {
    return image->is_current_texture(id);
}

void HudSprite::render() {
    image->render(position);
    if (amount_label) {
        amount_label->render_left();
    }
    if (bounded) {
        constexpr SDL_Color yellow = {235, 224, 70, 255};
        renderer.SetDrawColor(yellow.r, yellow.g, yellow.b, yellow.a);
        renderer.DrawRect(position-SDL2pp::Point{2,2}, position+size);
        renderer.SetDrawColor(0, 0, 0, 255);
    }
}

void HudSprite::bind() {
    bounded = true;
}

void HudSprite::unbind() {
    bounded = false;
}

uint8_t HudSprite::get_id() const {
    return image->get_id();
}
