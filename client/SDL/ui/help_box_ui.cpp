#include "help_box_ui.h"

#include "client/config/client_config.h"
HelpBoxUi::HelpBoxUi(SDL2pp::Renderer& renderer): renderer(renderer), must_be_rendered(false) {}

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
