#include "client_game.h"

#include <SDL2/SDL.h>
#include <SDL2pp/Renderer.hh>
#include <SDL2pp/SDL2pp.hh>
#include <SDL2pp/Texture.hh>
#include <SDL2pp/Window.hh>

#include "rate_timer.h"
#include "sprite.h"


ClientGame::ClientGame() :
    sdl(SDL2pp::SDL(SDL_INIT_VIDEO)),
    window(SDL2pp::Window("SDL2pp demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        640, 480,SDL_WINDOW_RESIZABLE)),
    renderer(SDL2pp::Renderer(window, -1, SDL_RENDERER_ACCELERATED)) {}


void ClientGame::run() {
    SDL2pp::Texture sprite1_sheet(renderer, DATA_PATH "/1431.png");
    Sprite user(renderer, sprite1_sheet, 256, 0, 27, 47);

    RateTimer timer(30);
    int iteration = 0;

    while (true) {
        int ret = pollEvents(user);
        if (ret == 1){break;}
        update_state_from_server();

        renderer.SetDrawColor(0,32,32);

        renderer.Clear();
        update_animation_frames(iteration, user);
        render_in_z_order(user);

        iteration = timer.calculate_next_iteration();
    }
}


int ClientGame::pollEvents(Sprite& user) const {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            return 1;
        }

        const SDL2pp::Point pos = user.get_position();
        SDL2pp::Point new_pos = pos;

        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_DOWN:
                    new_pos.SetY(pos.y+1);
                    user.update(WALK_DOWN, new_pos);
                    break;

                case SDLK_UP:
                    new_pos.SetY(pos.y-1);
                    user.update(WALK_UP, new_pos);
                    break;

                case SDLK_RIGHT:
                    new_pos.SetX(pos.x+1);
                    user.update(WALK_RIGHT, new_pos);
                    break;

                case SDLK_LEFT:
                    new_pos.SetX(pos.x-1);
                    user.update(WALK_LEFT, new_pos);
                    break;

                default:
                    break;
            }
        } else if (event.type == SDL_KEYUP) {
            user.update(IDLE, new_pos);
        }
    }
    return 0;
}

void ClientGame::update_state_from_server() {
    // Cuando se implemente la comunicación con servidor, debe tomar elementos de la queue de comandos
}

void ClientGame::update_animation_frames(const int it, Sprite& user) {
    // for (Sprite& entity : entities) {
    //     entity.update_frame(it);
    // }

    user.update_frame(it);
}

void ClientGame::render_in_z_order(const Sprite& user) {

    // for (Sprite& entity : entities) {
    //     entity.render();
    // }
    user.render();
    renderer.Present();
}
