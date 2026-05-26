#include "client_game.h"

#include <memory>
#include <utility>

#include <SDL2/SDL.h>
#include <SDL2pp/Renderer.hh>
#include <SDL2pp/SDL2pp.hh>
#include <SDL2pp/Window.hh>

#include "common/dto/events/moveevent.h"
#include "common/util/rate_timer.h"
#include "sprites/sprite.h"

#include "key_mapper.h"

#define FPS 30

ClientGame::ClientGame(ConnectionHandler& connection, std::string& player_name):
        sdl(SDL2pp::SDL(SDL_INIT_VIDEO)),
        window(SDL2pp::Window("SDL2pp demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480,
                              SDL_WINDOW_RESIZABLE)),
        renderer(SDL2pp::Renderer(window, -1, SDL_RENDERER_ACCELERATED)),
        player_name(std::move(player_name)),
        texture_pool(renderer),
        sprite_creator(renderer),
        connection(connection),
        key_being_pressed(SDLK_UNKNOWN) {
    connection.start();
}

void ClientGame::run() {

    RateTimer timer(FPS);
    int iteration = 0;

    while (true) {
        int ret = pollEvents();
        if (ret == 1)
            return;

        update_state_from_server();
        renderer.Clear();

        update_visuals(iteration);
        render_in_z_order();

        iteration = timer.calculate_next_iteration();
    }
}

int ClientGame::pollEvents() {
    SDL_Event event;
    bool key_was_pressed = false;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            connection.stop();
            return 1;
        }

        if (event.type == SDL_KEYDOWN) {
            key_was_pressed = true;
            handle_key_down(event);
        }

        // Esto no sirvió en esta condición, nunca se cumplía
        // key_was_pressed == event.key.keysym.sym &&
        if (event.type == SDL_KEYUP && KeyMapper::is_movement_key(event.key.keysym.sym)) {
            key_being_pressed = SDLK_UNKNOWN;
        }
    }

    // TODO esto definitivamente habría que modularizarlo/encapsularlo
    if (!key_was_pressed && KeyMapper::is_movement_key(key_being_pressed)) {
        connection.push_command(std::make_unique<MoveEventDTO>(KeyMapper::get_direction(key_being_pressed)));
    }

    return 0;
}

void ClientGame::update_state_from_server() {
    SnapshotDTO snapshot;
    bool updated = false;

    while (connection.try_pop_snapshot(snapshot)) {
        updated = true;
        for (const ActionDTO& action: snapshot.actions) {
            handle_action(action);
        }
    }

    if (!updated)
        return;
    update_players(snapshot.players_information);
}

void ClientGame::update_visuals(const int it) {
    for (auto& [name, entity]: players) {
        entity.update_visual_position();
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

void ClientGame::add_new_player(const PlayerInfoDTO& info) {
    Sprite user = sprite_creator.create_user(info);
    players.insert({{info.name, user}});
}

void ClientGame::handle_key_down(const SDL_Event& event) {
    assert(event.type == SDL_KEYDOWN);
    auto key_pressed = event.key.keysym.sym;

    //    if (key_pressed == key_being_pressed)
    //        return; // evito floodear al servidor
    //
    if (KeyMapper::is_movement_key(key_pressed)) {
        Direction direction_chosen = KeyMapper::get_direction(key_pressed);

        connection.push_command(std::make_unique<MoveEventDTO>(MoveEventDTO(direction_chosen)));
        key_being_pressed = key_pressed;
    }
}

void ClientGame::update_players(const std::vector<PlayerInfoDTO>& players_information) {
    for (const PlayerInfoDTO& player_info: players_information) {
        if (!players.contains(player_info.name)) {
            add_new_player(player_info);
        }

        SDL2pp::Point position(player_info.x, player_info.y);
        players.at(player_info.name).set_target_position(player_info.direction, position);
    }
}

void ClientGame::handle_action(const ActionDTO& action) {
    // TODO agregar todos los tipos que vayamos agregando
    switch (action.action) {
        case ActionType::DESPAWN:
            if (players.contains(action.despawn.player_despawned)) {
                players.extract(action.despawn.player_despawned);
            }
            break;
        default:
            break;
    }
}
