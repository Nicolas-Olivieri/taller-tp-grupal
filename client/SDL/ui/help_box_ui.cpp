#include "help_box_ui.h"

#include <string>

#include "client/config/client_config.h"
HelpBoxUi::HelpBoxUi(SpriteCreator& sprite_creator, SDL2pp::Renderer& renderer):
        config(ClientConfig::get().get_help_data()), renderer(renderer), current_page(HelpPage::NO_PAGE) {
    load_page(HelpPage::GENERAL, config.help_messages, sprite_creator);
    load_page(HelpPage::CLAN, config.clan_help_messages, sprite_creator);
    load_page(HelpPage::CHEAT, config.cheat_help_messages, sprite_creator);
    load_page(HelpPage::NPC, config.npc_help_messages, sprite_creator);
}

void HelpBoxUi::choose_page(const HelpPage& page) {
    assert(page != HelpPage::NO_PAGE);

    // Uso la general como cierre de todas
    if (current_page != HelpPage::NO_PAGE and page == HelpPage::GENERAL) {
        current_page = HelpPage::NO_PAGE;
        return;
    }

    current_page = page;
}

void HelpBoxUi::render() {
    if (current_page == HelpPage::NO_PAGE)
        return;

    assert(help_pages.contains(current_page));
    auto& help_texts = help_pages.at(current_page);

    const auto& client_config = ClientConfig::get();

    renderer.SetDrawBlendMode(SDL_BLENDMODE_BLEND);
    renderer.SetDrawColor(client_config.get_color_data().transparent_black);
    renderer.FillRect(client_config.get_ui_data().viewport);
    renderer.SetDrawBlendMode(SDL_BLENDMODE_NONE);

    for (auto& text: help_texts) text.render_left();
}

void HelpBoxUi::load_page(const HelpPage& page, const std::vector<std::string>& help_messages,
                          SpriteCreator& sprite_creator) {
    assert(not help_pages.contains(page));

    const auto& viewport = ClientConfig::get().get_ui_data().viewport;

    std::vector<TextSprite> help_new_page;
    help_new_page.reserve(help_messages.size());

    SDL2pp::Color white = ClientConfig::get().get_color_data().white;

    uint16_t y_offset = viewport.y + config.top_margin;
    const uint16_t x_offset = viewport.x + config.left_margin;
    const uint16_t line_spacing = config.line_spacing;
    const uint16_t msg_width = viewport.w - config.left_margin;
    const uint16_t msg_height = (viewport.h - config.top_margin) / config.max_msg_per_page;

    for (const std::string& msg: help_messages) {
        SDL2pp::Rect text_box(x_offset, y_offset, msg_width, msg_height);
        help_new_page.push_back(sprite_creator.create_sprite(text_box, msg, FontType::UI_MENU, white));
        y_offset += line_spacing;
    }

    help_pages.insert({page, std::move(help_new_page)});
}
