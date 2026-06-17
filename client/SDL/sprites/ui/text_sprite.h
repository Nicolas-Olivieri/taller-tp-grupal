#ifndef TEXT_SPRITE_H
#define TEXT_SPRITE_H
#include "client/SDL/fonts/font_manager.h"
#include "client/SDL/sprites/base/sprite.h"


class TextSprite : public Sprite {
private:
    SDL2pp::Renderer& renderer;

    std::string current_text;
    SDL2pp::Rect box;
    SDL2pp::Color color;
    SDL2pp::Font& font;
    std::optional<SDL2pp::Texture> texture;

public:
    TextSprite(SDL2pp::Renderer &renderer, SDL2pp::Rect box, const std::string &text, SDL2pp::Font& font,
               SDL2pp::Color color);

    void set_text(const std::string &new_text);

    void render();

    void render_left();

    std::string get_text();
};


#endif //TEXT_SPRITE_H
