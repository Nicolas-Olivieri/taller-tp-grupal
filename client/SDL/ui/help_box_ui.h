#ifndef HELP_BOX_UI_H
#define HELP_BOX_UI_H

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "client/SDL/sprite_creation/sprite_creator.h"
#include "client/SDL/sprites/ui/text_sprite.h"
#include "client/config/client_config.h"

enum class HelpPage { NO_PAGE, GENERAL, CLAN, CHEAT, NPC };

class HelpBoxUi {
    const HelpMsgData& config;
    SDL2pp::Renderer& renderer;
    HelpPage current_page;
    std::unordered_map<HelpPage, std::vector<TextSprite>> help_pages;

public:
    HelpBoxUi(SpriteCreator& sprite_creator, SDL2pp::Renderer& renderer);

    void choose_page(const HelpPage& page);

    void render();

private:
    void load_page(const HelpPage& page, const std::vector<std::string>& help_messages,
                   SpriteCreator& sprite_creator);
};


#endif  // HELP_BOX_UI_H
