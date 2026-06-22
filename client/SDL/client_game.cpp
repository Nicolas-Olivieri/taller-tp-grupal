#include "client_game.h"

#include <memory>
#include <regex>
#include <utility>

#include <SDL2/SDL.h>
#include <SDL2pp/Renderer.hh>
#include <SDL2pp/SDL2pp.hh>
#include <SDL2pp/Window.hh>

#include "../client_constants.h"
#include "client/config/client_config.h"
#include "client/util/key_mapper.h"
#include "common/dto/events/ally_related/deposit/deposit_gold_event.h"
#include "common/dto/events/ally_related/deposit/deposit_item_event.h"
#include "common/dto/events/ally_related/interact_event.h"
#include "common/dto/events/ally_related/shop/buy_event.h"
#include "common/dto/events/ally_related/shop/sell_event.h"
#include "common/dto/events/ally_related/withdraw/withdraw_gold_event.h"
#include "common/dto/events/ally_related/withdraw/withdraw_item_event.h"
#include "common/dto/events/chat/chatevent.h"
#include "common/dto/events/clan/clan_found_event.h"
#include "common/dto/events/clan/clan_join_event.h"
#include "common/dto/events/clan/clan_remove_player_event.h"
#include "common/dto/events/clan/clan_request_response_event.h"
#include "common/dto/events/drop_item_event.h"
#include "common/dto/events/movement/moveevent.h"
#include "common/dto/events/unequip_item_event.h"
#include "common/dto/events/use_item_event.h"
#include "common/util/rate_timer.h"

#include "camera.h"


ClientGame::ClientGame(ConnectionHandler& connection, std::string& player_name, const std::string& resolution,
                       AudioManager& audio_manager, FontManager& font_manager):
        config(ClientConfig::get().load_resolution_data(resolution)),
        sdl(SDL2pp::SDL(SDL_INIT_VIDEO)),
        window(SDL2pp::Window("Argentum Online", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              config.window_width, config.window_height, SDL_WINDOW_BORDERLESS)),
        renderer(SDL2pp::Renderer(window, -1, SDL_RENDERER_ACCELERATED)),
        connection(connection),
        player_name(player_name),
        world(renderer, connection.receive_map(), player_name, audio_manager, font_manager),
        camera(initialize_world_and_camera()),
        ui(renderer, player_name, font_manager),
        keep_running(true),
        just_restored(false),
        is_fullscreen(false),
        key_handler(player_name, connection, ui) {
    SDL_SetWindowHitTest(window.Get(), hit_test_callback, this);
}

void ClientGame::run() {

    RateTimer timer(ClientConfig::get().get_fps());
    int iteration = 0;

    while (keep_running) {
        pollEvents();
        if (not keep_running)
            return;

        update_state_from_server();
        if (not keep_running)
            return;

        renderer.Clear();

        world.update_visuals(iteration);
        camera.update_position();

        render_ui_and_world();
        renderer.Present();

        iteration = timer.calculate_next_iteration();
    }
}

Camera ClientGame::initialize_world_and_camera() {
    // 1. Recibe el mundo
    // 2. Recibe primer snapshot con yo adentro?
    connection.start();

    while (true) {
        const SnapshotDTO snapshot = connection.pop_snapshot();
        std::vector<PlayerInfoDTO> info = snapshot.players_information;

        auto it = std::find_if(info.begin(), info.end(), [this](const PlayerInfoDTO& player_info) {
            return player_info.name == player_name;
        });

        if (it != info.end()) {
            world.add_new_player(*it);
            world.update_players(info);
            break;
        }
    }
    PlayerSprite& user = world.get_client_player();
    SDL2pp::Rect& world_size = world.get_world_size();
    return {config.viewport.GetW(), config.viewport.GetH(), world_size, user};
}

void ClientGame::pollEvents() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESTORED) {
            just_restored = true;
        }

        if (event.type == SDL_MOUSEBUTTONDOWN) {
            handle_mouse_click(event);
        }

        if (event.type == SDL_MOUSEBUTTONUP && just_restored) {
            just_restored = false;
            handle_mouse_click(event);
        }

        if (event.type == SDL_MOUSEWHEEL)
            handle_mouse_wheel(event);

        if (key_handler.is_chat_active()) {
            key_handler.handle_chat_event(event);
            continue;
        }

        if (event.type == SDL_KEYDOWN) {
            key_handler.handle_key_down(event);
        }

        if (key_handler.is_releasing_key(event)) {
            key_handler.release_key();
        }
    }

    if (not keep_running) {
        connection.stop();
        return;
    }

    key_handler.handle_continuous_movement();
}


void ClientGame::update_state_from_server() {
    if (connection.is_finished()) {
        keep_running = false;
        connection.stop();
        return;
    }

    SnapshotDTO snapshot;
    bool updated = false;

    while (connection.try_pop_snapshot(snapshot)) {
        updated = true;
        world.handle_actions(snapshot.actions);
        ui.update_chat(snapshot.actions);
    }

    if (!updated)
        return;
    world.update_players(snapshot.players_information);
    world.update_creatures(snapshot.creatures_information);
    world.update_loot(snapshot.loot_information);
    world.erase_finished_effects();
    ui.update_player_state(snapshot.players_information);
}


void ClientGame::handle_mouse_click(const SDL_Event& event) {
    assert(event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP);
    if (is_inside_viewport(event.button.x, event.button.y, config.viewport)) {
        handle_game_click(event);
    } else {
        handle_ui_click(event);
    }
}

void ClientGame::render_ui_and_world() {
    ui.render(key_handler.get_chat_text(), key_handler.is_chat_active());

    renderer.SetViewport(config.viewport);
    world.render_in_z_order(camera);

    renderer.SetViewport(SDL2pp::NullOpt);

    ui.render_help();
}

bool ClientGame::is_inside_viewport(int x, int y, const SDL2pp::Rect& viewport) {
    return x >= viewport.x && x <= (viewport.x + viewport.w) && y >= viewport.y &&
           y <= (viewport.y + viewport.h);
}

void ClientGame::handle_ui_click(const SDL_Event& event) {
    int x = event.button.x;
    int y = event.button.y;

    const int inventory_slot_index = ui.get_inventory_slot_at(x, y);
    if (inventory_slot_index != -1) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            ui.bind_item(inventory_slot_index);

        } else if (event.button.button == SDL_BUTTON_RIGHT) {
            auto item_id = ui.get_item_in_inventory_slot(inventory_slot_index);
            if (item_id.has_value())
                connection.push_command(std::make_unique<UseItemEventDTO>(item_id.value()));
        }

        return;
    }

    const int equipment_slot_index = ui.get_equipment_slot_at(x, y);
    if (equipment_slot_index != -1) {
        if (event.button.button == SDL_BUTTON_RIGHT) {
            auto item_id = ui.get_item_in_equipment_slot(equipment_slot_index);
            if (item_id.has_value())
                connection.push_command(std::make_unique<UnequipItemEventDTO>(item_id.value()));
        }

        return;
    }

    if (event.button.button == SDL_BUTTON_LEFT) {
        // Clic izquierdo sobre el chat
        if (is_inside_viewport(x, y, config.chat_icon)) {
            key_handler.toggle_chat();
        } else if (is_inside_viewport(x, y, config.minimize_button)) {
            window.Minimize();

        } else if (is_inside_viewport(x, y, config.maximize_button)) {
            if (is_fullscreen) {
                SDL_SetWindowFullscreen(window.Get(), 0);
                is_fullscreen = false;
            } else {
                SDL_SetWindowFullscreen(window.Get(), SDL_WINDOW_FULLSCREEN_DESKTOP);
                is_fullscreen = true;
            }

        } else if (is_inside_viewport(x, y, config.close_button)) {
            keep_running = false;
        }
    }
}

void ClientGame::handle_game_click(const SDL_Event& event) {
    const uint16_t tile_size = ClientConfig::get().get_tile_size();
    int game_click_x = event.button.x - config.viewport.x;
    int game_click_y = event.button.y - config.viewport.y;

    if (event.button.button == SDL_BUTTON_LEFT) {
        const uint16_t target_x = (camera.get_view().GetX() + game_click_x) / tile_size;
        const uint16_t target_y = (camera.get_view().GetY() + game_click_y) / tile_size;
        connection.push_command(std::make_unique<InteractEventDTO>(target_x, target_y));
    }
}

SDL_HitTestResult ClientGame::hit_test_callback(SDL_Window*, const SDL_Point* area, void* data) {
    ClientGame* game = static_cast<ClientGame*>(data);

    if (game->is_inside_viewport(area->x, area->y, game->config.header_bar)) {
        return SDL_HITTEST_DRAGGABLE;
    }

    return SDL_HITTEST_NORMAL;
}

void ClientGame::handle_mouse_wheel(const SDL_Event& event) {
    int mouse_x;
    int mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);

    if (!ui.is_over_chat(mouse_x, mouse_y))
        return;

    if (event.wheel.y > 0)
        ui.chat_scroll_up();

    if (event.wheel.y < 0)
        ui.chat_scroll_down();
}
