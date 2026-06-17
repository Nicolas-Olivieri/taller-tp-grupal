#include "text_sprite.h"

TextSprite::TextSprite(SDL2pp::Renderer &renderer, const SDL2pp::Rect box, const std::string &text,
                       SDL2pp::Font& font, const SDL2pp::Color color) :
    Sprite(box.GetTopLeft(), box.GetSize(), {0, 0}), renderer(renderer),
    current_text(text), box(box), color(color), font(font) {
    if (!text.empty()) {
        texture = SDL2pp::Texture(renderer, font.RenderUTF8_Solid(current_text, color));
    }
}


void TextSprite::set_text(const std::string &new_text) {
    if (new_text == current_text) {
        return;
    }

    current_text = new_text;
    if (current_text.empty()) {
        texture = std::nullopt;
    } else {
        texture = SDL2pp::Texture(renderer, font.RenderUTF8_Solid(current_text, color));
    }
}

void TextSprite::render() {
    if (current_text.empty()) {
        return;
    }

    const int text_w = std::min(texture.value().GetWidth(), box.w);
    const int text_h = std::min(texture.value().GetHeight(), box.h);

    SDL2pp::Rect centered_box = {box.x + (box.w - text_w) / 2,
                                 box.y + (box.h - text_h) / 2, text_w, text_h};

    renderer.Copy(texture.value(), SDL2pp::NullOpt, centered_box);
}

void TextSprite::render_left() {
    if (current_text.empty()) {
        return;
    }
    const int text_w = texture.value().GetWidth();
    const int text_h = texture.value().GetHeight();

    const int text_x = position.x;
    const int text_y = position.y;

    renderer.Copy(texture.value(), SDL2pp::NullOpt, SDL2pp::Rect(text_x, text_y, text_w, text_h));
}

std::string TextSprite::get_text() {
    return current_text;
}