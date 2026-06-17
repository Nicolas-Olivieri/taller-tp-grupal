#include "text_sprite.h"

#include <algorithm>
#include <string>

TextSprite::TextSprite(SDL2pp::Renderer& renderer, const SDL2pp::Rect box, const std::string& text,
                       SDL2pp::Font& font, const SDL2pp::Color color):
        Sprite(box.GetTopLeft(), box.GetSize(), {0, 0}),
        renderer(renderer),
        current_text(text),
        box(box),
        color(color),
        font(font) {
    if (!text.empty()) {
        texture = SDL2pp::Texture(renderer, font.RenderUTF8_Solid(current_text, color));
    }
}


void TextSprite::set_text(const std::string& new_text) {
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

void TextSprite::set_color(const SDL2pp::Color new_color) {
    color = new_color;
    if (!current_text.empty()) {
        texture = SDL2pp::Texture(renderer, font.RenderUTF8_Solid(current_text, color));
    }
}

void TextSprite::render() {
    if (current_text.empty()) {
        return;
    }

    const int text_w = std::min(texture.value().GetWidth(), box.w);
    const int text_h = std::min(texture.value().GetHeight(), box.h);

    SDL2pp::Rect centered_box = {box.x + (box.w - text_w) / 2, box.y + (box.h - text_h) / 2, text_w, text_h};

    renderer.Copy(texture.value(), SDL2pp::NullOpt, centered_box);
}

void TextSprite::render_left() {
    if (current_text.empty()) {
        return;
    }

    SDL2pp::Point size = texture.value().GetSize();
    cut_text_if_necessary(size, box.w);

    renderer.Copy(texture.value(), SDL2pp::NullOpt, SDL2pp::Rect(position, size));
}

std::string TextSprite::get_text() const { return current_text; }

SDL_Color TextSprite::get_color() const { return color; }

void TextSprite::cut_text_if_necessary(SDL2pp::Point& text_size, const int max_width) {
    if (text_size.GetX() > max_width) {
        text_size.SetX(max_width);
    }
}
