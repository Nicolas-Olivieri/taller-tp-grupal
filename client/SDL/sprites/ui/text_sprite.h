#ifndef TEXT_SPRITE_H
#define TEXT_SPRITE_H
#include <string>

#include "client/SDL/fonts/font_manager.h"
#include "client/SDL/sprites/base/sprite.h"

class TextSprite: public Sprite {
private:
    SDL2pp::Renderer& renderer;

    std::string current_text;
    SDL2pp::Rect box;
    SDL2pp::Color color;
    SDL2pp::Font& font;
    std::optional<SDL2pp::Texture> texture;

    void cut_text_if_necessary(SDL2pp::Point& text_size, int max_width);

public:
    TextSprite(SDL2pp::Renderer& renderer, SDL2pp::Rect box, const std::string& text, SDL2pp::Font& font,
               SDL2pp::Color color);

    void render();
    void render_left();

    void set_text(const std::string& new_text);
    void set_color(SDL2pp::Color new_color);

    std::string get_text() const;
    SDL_Color get_color() const;
};


#endif  // TEXT_SPRITE_H
