#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include <unordered_map>

#include <SDL2pp/Font.hh>
#include <SDL2pp/SDLTTF.hh>

#include "client/TOML/font_parser.h"


class FontManager {
private:
    SDL2pp::SDLTTF ttf;
    std::unordered_map<FontType, SDL2pp::Font> fonts;

public:
    FontManager();

    SDL2pp::Font& get_font(const FontType& type);

private:
    void load_font_config();
};


#endif  // FONT_MANAGER_H
