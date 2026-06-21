#include "help_box_ui.h"

#include <string>

#include "client/config/client_config.h"
HelpBoxUi::HelpBoxUi(SpriteCreator& sprite_creator, SDL2pp::Renderer& renderer):
        renderer(renderer), must_be_rendered(false) {
    const auto& config = ClientConfig::get().get_help_data();
    const std::vector<std::string>& help_messages = config.help_messages;
    const auto& viewport = ClientConfig::get().get_ui_data().viewport;

    help_texts.reserve(help_messages.size());

    SDL2pp::Color white = ClientConfig::get().get_color_data().white;

    uint16_t y_offset = viewport.y + config.top_margin;
    const uint16_t x_offset = viewport.x + config.left_margin;
    const uint16_t line_spacing = config.line_spacing;

    for (const std::string& msg: help_messages) {
        SDL2pp::Rect text_box(x_offset, y_offset, 600, 20);
        help_texts.push_back(sprite_creator.create_sprite(text_box, msg, FontType::UI_MENU, white));
        y_offset += line_spacing;
    }
}

void HelpBoxUi::toggle_visibility() { must_be_rendered = !must_be_rendered; }

void HelpBoxUi::render() {
    if (not must_be_rendered)
        return;

    const auto& config = ClientConfig::get();

    renderer.SetDrawBlendMode(SDL_BLENDMODE_BLEND);
    renderer.SetDrawColor(config.get_color_data().transparent_black);
    renderer.FillRect(config.get_ui_data().viewport);
    renderer.SetDrawBlendMode(SDL_BLENDMODE_NONE);

    for (auto& text: help_texts) {
        text.render_left();
    }
}
