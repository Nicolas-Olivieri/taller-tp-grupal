#include "progress_bar_sprite.h"


ProgressBarSprite::ProgressBarSprite(SpriteLayer &&image, const SDL2pp::Point &position, const SDL2pp::Point &size,
                                     const SDL2pp::Rect box, const size_t current, const size_t max) :
    Sprite(position, size, SDL2pp::Point(0,0)),
    image(std::move(image)), box(box), max(max), current(current), label(std::nullopt) {}

ProgressBarSprite::ProgressBarSprite(SpriteLayer &&image, TextSprite &&label, const SDL2pp::Point &position, const SDL2pp::Point &size,
                                     const SDL2pp::Rect box, const size_t current, const size_t max) :
    Sprite(position, size, SDL2pp::Point(0,0)),
    image(std::move(image)), box(box), max(max), current(current), label(std::move(label)) {}


void ProgressBarSprite::update_values(const size_t new_current, const size_t new_max) {
    current = new_current;
    max = new_max;

    if (label) {
        label->set_text(std::format("{} / {}", current, max));
    }
}

void ProgressBarSprite::render() {
    if (max == 0) return;

    const float ratio = static_cast<float>(current) / static_cast<float>(max);
    const int filled_w = static_cast<int>(box.w * ratio);

    const SDL2pp::Rect src(0, 0, filled_w, image.get_frame_area().GetH());
    const SDL2pp::Rect dest(box.x, box.y, filled_w, box.h);

    image.render(src, dest);

    if (label) {
        label->render();
    }
}
