#include "user_interface.h"

#include <cassert>
UserInterface::UserInterface(SDL2pp::Renderer& renderer, std::string& player_name):
        renderer(renderer),
        ui_texture(renderer, DATA_PATH "/interfaz_principal.bmp"),
        player_name(player_name) {}

void UserInterface::render() { renderer.Copy(ui_texture, SDL2pp::NullOpt, SDL2pp::NullOpt); }

void UserInterface::render_fields() { assert(false); }

void UserInterface::update_player_state(const std::vector<PlayerInfoDTO>& /* players_information */) {
    assert(false);
}

void UserInterface::update_chat(const std::vector<ActionDTO>& /* actions */) { assert(false); }
