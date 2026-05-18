#ifndef SDL_H
#define SDL_H

#include <map>
#include <string>
#include <vector>

#include <SDL2pp/Renderer.hh>
#include <SDL2pp/SDL2pp.hh>

#include "sprites/sprite.h"
#include "sprites/sprite_creator.h"
#include "sprites/texture_pool.h"

struct ServerMsgMock {
    std::string player_name;
    uint16_t x;
    uint16_t y;
    Direction action;
};

class ClientGame {
private:
    SDL2pp::SDL sdl;
    SDL2pp::Window window;
    SDL2pp::Renderer renderer;

    std::vector<ServerMsgMock> mockedQueue;

    TexturePool texture_pool;
    SpriteCreator sprite_creator;
    std::map<std::string, Sprite> players;

public:
    ClientGame();

    void run();

    int pollEvents();

    int pollEvents(Sprite& user) const;

    void update_state_from_server();

    void update_animation_frames(int it);

    void render_in_z_order();
};


#endif
