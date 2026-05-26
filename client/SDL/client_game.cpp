#include "client_game.h"

#include <memory>
#include <utility>

#include <SDL2/SDL.h>
#include <SDL2pp/Renderer.hh>
#include <SDL2pp/SDL2pp.hh>
#include <SDL2pp/Window.hh>

#include "../client_constants.h"
#include "common/dto/events/interact_event.h"
#include "common/dto/events/moveevent.h"
#include "common/util/rate_timer.h"
#include "sprites/sprite.h"

#include "camera.h"
#include "key_mapper.h"


ClientGame::ClientGame(ConnectionHandler& connection, std::string& player_name):
        sdl(SDL(SDL_INIT_VIDEO)),
        window(Window("SDL2pp demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                      SCREEN_HEIGHT, 0)),
        renderer(Renderer(window, -1, SDL_RENDERER_ACCELERATED)),
        connection(connection),
        player_name(player_name),
        world(renderer, player_name),
        key_being_pressed(SDLK_UNKNOWN) {
    connection.start();
}

void ClientGame::run() {

    RateTimer timer(FPS);
    int iteration = 0;

    Camera camera = initialize_world_and_camera();
    while (true) {
        int ret = pollEvents();
        if (ret == 1)
            return;

        update_state_from_server();
        camera.update_position();
        renderer.Clear();

        world.update_visuals(iteration);
        world.render_in_z_order(camera);

        iteration = timer.calculate_next_iteration();
    }
}

Camera ClientGame::initialize_world_and_camera() {
    // 1. Recibe el mundo
    // 2. Recibe primer snapshot con yo adentro?

    while (true) {
        const SnapshotDTO snapshot = connection.pop_snapshot();
        std::vector<PlayerInfoDTO> info = snapshot.players_information;

        auto it = std::find_if(info.begin(), info.end(), [this](const PlayerInfoDTO& player_info) {
            return player_info.name == player_name;
        });

        if (it != info.end()) {
            std::cout << it->name << std::endl;
            world.update_players(info);
            break;
        }
    }
    Sprite& user = world.get_client_player();
    Rect& world_size = world.get_world_size();
    return {SCREEN_WIDTH, SCREEN_HEIGHT, world_size, user};
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

        if (event.type == SDL_MOUSEBUTTONDOWN) {
            handle_mouse_click(event);
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
        world.handle_actions(snapshot.actions);
    }

    if (!updated)
        return;
    world.update_players(snapshot.players_information);
    // TODO añadir el resto del manejo de sprites
}


void ClientGame::handle_key_down(const SDL_Event& event) {
    assert(event.type == SDL_KEYDOWN);
    auto key_pressed = event.key.keysym.sym;

    if (KeyMapper::is_movement_key(key_pressed)) {
        Direction direction_chosen = KeyMapper::get_direction(key_pressed);

        connection.push_command(std::make_unique<MoveEventDTO>(MoveEventDTO(direction_chosen)));
        key_being_pressed = key_pressed;
    }
}

void ClientGame::handle_mouse_click(const SDL_Event& event) {
    if (event.button.button == SDL_BUTTON_LEFT) {
        const uint16_t target_x = event.button.x / TILE_SIZE;
        const uint16_t target_y = event.button.y / TILE_SIZE;
        connection.push_command(std::make_unique<InteractEventDTO>(target_x, target_y));
    }
}
