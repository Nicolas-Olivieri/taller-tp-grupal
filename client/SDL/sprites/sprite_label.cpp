#include "sprite_label.h"

#include "client/SDL/user_interface.h"
#include "common/dto/snapshot/info/creatureinfo.h"

#define LABEL_HEALTH_BAR "/barra_vida.bmp"


SpriteLabel::SpriteLabel(SDL2pp::Renderer& renderer, FontManager& font_manager,
                         const PlayerInfoDTO& player_info):
        renderer(renderer),
        font_manager(font_manager),
        name(player_info.name),
        xp_level(player_info.stats.xp_level),
        clan(player_info.clan_name),
        health_texture(renderer, DATA_PATH LABEL_HEALTH_BAR),
        current_health(player_info.stats.current_health),
        max_health(player_info.stats.max_health) {
    refresh_name_level_texture();
    refresh_clan_texture();
}


SpriteLabel::SpriteLabel(SDL2pp::Renderer& renderer, FontManager& font_manager,
                         const CreatureInfoDTO& creature_info):
        renderer(renderer),
        font_manager(font_manager),
        name(id_to_string.at(creature_info.creature)),
        xp_level(creature_info.stats.xp_level),
        health_texture(renderer, DATA_PATH LABEL_HEALTH_BAR),
        current_health(creature_info.stats.current_health),
        max_health(creature_info.stats.max_health) {
    refresh_name_level_texture();
}


void SpriteLabel::update(const PlayerInfoDTO& player_info) {
    current_health = player_info.stats.current_health;
    max_health = player_info.stats.max_health;

    if (name != player_info.name || xp_level != player_info.stats.xp_level) {
        name = player_info.name;
        xp_level = player_info.stats.xp_level;
        refresh_name_level_texture();
    }

    if (clan != player_info.clan_name) {
        clan = player_info.clan_name;
        refresh_clan_texture();
    }
}


void SpriteLabel::update(const CreatureInfoDTO& creature_info) {
    current_health = creature_info.stats.current_health;
    max_health = creature_info.stats.max_health;

    if (xp_level != creature_info.stats.xp_level) {
        xp_level = creature_info.stats.xp_level;
        refresh_name_level_texture();
    }
}


void SpriteLabel::render(const SDL2pp::Point render_position, const SDL2pp::Point sprite_size) {
    const int center_x = render_position.GetX() + sprite_size.GetX() / 2;
    const int top_y = render_position.GetY() - 4;
    const int bottom_y = render_position.GetY() + sprite_size.GetY();

    const SDL2pp::Rect health_rect = {center_x - 32, top_y, 64, 8};
    render_health(health_rect);

    if (name_level_texture) {
        const int name_y = (current_health > 0) ?
                                   top_y - name_level_texture->GetHeight() - health_rect.GetH() :
                                   top_y - name_level_texture->GetHeight();

        render_cached_texture(*name_level_texture.get(), center_x, name_y);
    }

    if (clan_texture) {
        render_cached_texture(*clan_texture.get(), center_x, bottom_y);
    }
}


void SpriteLabel::refresh_name_level_texture() {
    const std::string text = name + " - " + std::to_string(xp_level);
    name_level_texture = std::make_unique<SDL2pp::Texture>(
            renderer, font_manager.get_font(FontType::LABEL_NAME_LEVEL).RenderText_Solid(text, white));
}


void SpriteLabel::refresh_clan_texture() {
    if (clan.empty()) {
        clan_texture.reset();
        return;
    }

    const std::string text = "<" + clan + ">";
    clan_texture = std::make_unique<SDL2pp::Texture>(
            renderer, font_manager.get_font(FontType::LABEL_CLAN).RenderText_Solid(text, yellow));
}


void SpriteLabel::render_health(const SDL2pp::Rect& health_rect) {
    if (current_health == 0 || max_health == 0)
        return;

    const float ratio = static_cast<float>(current_health) / static_cast<float>(max_health);
    const int filled_w = static_cast<int>(health_rect.w * ratio);

    renderer.Copy(health_texture, SDL2pp::Rect(0, 0, filled_w, health_texture.GetHeight()),
                  SDL2pp::Rect(health_rect.x, health_rect.y, filled_w, health_rect.h));
}


void SpriteLabel::render_cached_texture(SDL2pp::Texture& texture, const int center_x, const int y_pos) const {
    SDL2pp::Rect rect(center_x - texture.GetWidth() / 2, y_pos, texture.GetWidth(), texture.GetHeight());
    renderer.Copy(texture, SDL2pp::NullOpt, rect);
}
