#ifndef HELP_BOX_UI_H
#define HELP_BOX_UI_H


#include <vector>

#include "client/SDL/sprites/ui/text_sprite.h"

class HelpBoxUi {
    SDL2pp::Renderer& renderer;
    bool must_be_rendered;
    std::vector<TextSprite> help_texts;

public:
    explicit HelpBoxUi(SDL2pp::Renderer& renderer);

    void toggle_visibility();

    void render();
};


#endif  // HELP_BOX_UI_H
