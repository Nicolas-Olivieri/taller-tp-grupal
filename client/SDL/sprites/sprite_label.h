#ifndef SPRITE_LABEL_H
#define SPRITE_LABEL_H

#include <memory>
#include <string>
#include <unordered_map>

#include <SDL2pp/Renderer.hh>
#include <SDL2pp/Texture.hh>

#include "client/SDL/fonts/font_manager.h"
#include "common/dto/snapshot/info/creatureinfo.h"
#include "common/dto/snapshot/info/playerinfo.h"


class SpriteLabel {
private:
    // TODO: Refactorizar inicialización de colores (se me ocurre un ColorManager o ColorConfig)
    SDL_Color yellow = {235, 224, 70, 255};
    SDL_Color white = {255, 255, 255, 255};

    SDL2pp::Renderer& renderer;
    FontManager& font_manager;

    std::string name;
    uint8_t xp_level;
    std::string clan;

    SDL2pp::Texture health_texture;
    uint16_t current_health;
    uint16_t max_health;

    std::unique_ptr<SDL2pp::Texture> name_level_texture;
    std::unique_ptr<SDL2pp::Texture> clan_texture;

public:
    SpriteLabel(SDL2pp::Renderer& renderer, FontManager& font_manager, const PlayerInfoDTO& player_info);

    SpriteLabel(SDL2pp::Renderer& renderer, FontManager& font_manager, const CreatureInfoDTO& creature_info);

    void update(const PlayerInfoDTO& player_info);

    void update(const CreatureInfoDTO& creature_info);

    void render(SDL2pp::Point render_position, SDL2pp::Point sprite_size);

private:
    void refresh_name_level_texture();

    void refresh_clan_texture();

    void render_health(const SDL2pp::Rect& health_rect);

    void render_cached_texture(SDL2pp::Texture& texture, int center_x, int y_pos) const;
};


#endif  // SPRITE_LABEL_H
