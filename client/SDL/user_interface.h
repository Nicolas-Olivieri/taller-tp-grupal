#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H


#include <string>
#include <vector>

#include "SDL2pp/Renderer.hh"
#include "SDL2pp/Texture.hh"
#include "common/dto/snapshot/actions/action.h"
#include "common/dto/snapshot/info/playerinfo.h"

class UserInterface {

    SDL2pp::Renderer& renderer;
    // TODO convertirlo en un sprite
    SDL2pp::Texture ui_texture;
    std::string& player_name;

public:
    UserInterface(SDL2pp::Renderer& renderer, std::string& player_name);

    void render();

    // TODO capaz conviene que el render lo haga todo solo, todavía no lo sé
    void render_fields();

    void update_player_state(const std::vector<PlayerInfoDTO>& players_information);

    void update_chat(const std::vector<ActionDTO>& actions);
};


#endif  // USER_INTERFACE_H
