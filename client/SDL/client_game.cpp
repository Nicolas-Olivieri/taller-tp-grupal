#include "client_game.h"

#include <SDL2/SDL.h>
#include <SDL2pp/Renderer.hh>
#include <SDL2pp/SDL2pp.hh>
#include <SDL2pp/Window.hh>

#include "common/util/rate_timer.h"
#include "sprites/sprite.h"

#define TILE_SIZE 25

ClientGame::ClientGame() :
    sdl(SDL2pp::SDL(SDL_INIT_VIDEO)),
    window(SDL2pp::Window("SDL2pp demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        640, 480,SDL_WINDOW_RESIZABLE)),
    renderer(SDL2pp::Renderer(window, -1, SDL_RENDERER_ACCELERATED)),
    texture_pool(renderer),
    sprite_creator(renderer)
{}


void ClientGame::run() {
    // MOCK ::::::::::::::::::
    Sprite user = sprite_creator.create_user();
    ServerMsgMock msg("dummy", 0,0, IDLE);

    players.insert({{"dummy", user}});

    mockedQueue.push_back(msg);
    // ::::::::::::::::::::::::

    RateTimer timer(24);
    int iteration = 0;

    while (true) {
        int ret = pollEvents();
        if (ret == 1) return;

        update_state_from_server();

        renderer.Clear();

        update_animation_frames(iteration);
        render_in_z_order();

        iteration = timer.calculate_next_iteration();
    }
}


int ClientGame::pollEvents() {
    SDL_Event event;
    Sprite& user = players.at("dummy");
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            return 1;
        }

        const SDL2pp::Point pos = user.get_position();

        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_DOWN:
                    mockedQueue[0] = ServerMsgMock("dummy", pos.x, pos.y+TILE_SIZE, DOWN);
                    break;

                case SDLK_UP:
                    mockedQueue[0] = ServerMsgMock("dummy", pos.x, pos.y-TILE_SIZE, UP);
                    break;

                case SDLK_RIGHT:
                    mockedQueue[0] = ServerMsgMock("dummy", pos.x+TILE_SIZE, pos.y, RIGHT);
                    break;

                case SDLK_LEFT:
                    mockedQueue[0] = ServerMsgMock("dummy", pos.x-TILE_SIZE, pos.y, LEFT);
                    break;

                default:
                    break;
            }
        }
    }
    return 0;
}

void ClientGame::update_state_from_server() {
    for (const ServerMsgMock& msg : mockedQueue) {
        if (players.find(msg.player_name) != players.end()) {
            players.at(msg.player_name).update_position(msg.action, SDL2pp::Point(msg.x, msg.y));
        }
    }
}

void ClientGame::update_animation_frames(const int it) {
    for (auto& [name, entity]: players) {
        entity.update_frame(it);
    }
}

void ClientGame::render_in_z_order() {
    renderer.SetDrawColor(0,32,32);
    for (auto& [name, entity] : players) {
        entity.render();
    }
    renderer.Present();
}
