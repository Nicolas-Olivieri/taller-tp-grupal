#ifndef SDL_H
#define SDL_H

#include <SDL2pp/Renderer.hh>
#include <SDL2pp/SDL2pp.hh>

#include "sprite.h"


class ClientGame {
private:
    SDL2pp::SDL sdl;
    SDL2pp::Window window;
    SDL2pp::Renderer renderer;


    std::vector<Sprite> entities;

public:
    ClientGame();

    void run();

    int pollEvents(Sprite &user) const;

    void update_state_from_server();

    void update_animation_frames(int it, Sprite &user);

    void render_in_z_order(const Sprite &user);
};


#endif
