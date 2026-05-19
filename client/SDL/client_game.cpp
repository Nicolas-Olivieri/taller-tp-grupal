#include "client_game.h"

#include <memory>

#include <SDL2/SDL.h>
#include <SDL2pp/Renderer.hh>
#include <SDL2pp/SDL2pp.hh>
#include <SDL2pp/Window.hh>

#include "common/dto/events/moveevent.h"
#include "common/util/rate_timer.h"
#include "sprites/sprite.h"

#include "key_mapper.h"

#define TILE_SIZE 25

ClientGame::ClientGame(ConnectionHandler& connection):
        sdl(SDL2pp::SDL(SDL_INIT_VIDEO)),
        window(SDL2pp::Window("SDL2pp demo", SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED, 640, 480,
                              SDL_WINDOW_RESIZABLE)),
        renderer(SDL2pp::Renderer(window, -1, SDL_RENDERER_ACCELERATED)),
        texture_pool(renderer),
        sprite_creator(renderer),
        connection(connection) {
    connection.start();
}

void ClientGame::run() {

    RateTimer timer(24);
    int iteration = 0;

    while (true) {
        int ret = pollEvents();
        if (ret == 1)
            return;

        update_state_from_server();

        renderer.Clear();

        update_animation_frames(iteration);
        render_in_z_order();

        iteration = timer.calculate_next_iteration();
    }
}

int ClientGame::pollEvents() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            connection.stop();
            return 1;
        }

        if (event.type == SDL_KEYDOWN) {
            handle_key_down(event);
        }
    }
    return 0;
}

void ClientGame::update_state_from_server() {
    SnapshotDTO snapshot;

    if (!connection.try_pop_snapshot(snapshot)) {
        return;
    }

    for (const ActionDTO& action: snapshot.actions) {
        handle_action(action);
    }

    update_players(snapshot.players_information);
}

void ClientGame::update_animation_frames(const int it) {
    for (auto& [name, entity]: players) {
        entity.update_frame(it);
    }
}

void ClientGame::render_in_z_order() {
    renderer.SetDrawColor(0, 32, 32);
    for (auto& [name, entity]: players) {
        entity.render();
    }
    renderer.Present();
}

void ClientGame::add_new_player(const AppearanceDTO& appearance) {
    Sprite user = sprite_creator.create_user(appearance);
    players.insert({{appearance.name, user}});
}

void ClientGame::handle_key_down(const SDL_Event& event) {
    assert(event.type == SDL_KEYDOWN);
    auto key_pressed = event.key.keysym.sym;

    if (KeyMapper::is_movement_key(key_pressed)) {
        Direction direction_chosen = KeyMapper::get_direction(key_pressed);

        connection.push_command(
                std::make_unique<MoveEventDTO>(MoveEventDTO(direction_chosen)));
        return;
    }
}

void ClientGame::update_players(
        const std::vector<PlayerInfoDTO>& players_information) {
    for (const PlayerInfoDTO& player_info: players_information) {
        if (!players.contains(player_info.name)) {
            continue;
        }

        players.at(player_info.name)
                .update_position(player_info.direction,
                                 SDL2pp::Point(player_info.x * TILE_SIZE,
                                               player_info.y * TILE_SIZE));
    }
}

void ClientGame::handle_action(const ActionDTO& action) {
    // TODO agregar todos los tipos que vayamos agregando
    switch (action.action) {
        case ActionType::APPEARANCE:
            add_new_player(action.appearance);
            break;
        default:
            break;
    }
}
