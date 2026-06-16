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
#include "common/dto/events/ally_related/deposit/deposit_gold_event.h"
#include "common/dto/events/ally_related/deposit/deposit_item_event.h"
#include "common/dto/events/ally_related/interact_event.h"
#include "common/dto/events/ally_related/shop/buy_event.h"
#include "common/dto/events/ally_related/shop/sell_event.h"
#include "common/dto/events/ally_related/withdraw/withdraw_gold_event.h"
#include "common/dto/events/ally_related/withdraw/withdraw_item_event.h"
#include "common/dto/events/chat/chatevent.h"
#include "common/dto/events/cheat/cheat_experience_set_event.h"
#include "common/dto/events/cheat/cheat_get_item_event.h"
#include "common/dto/events/cheat/cheat_gold_gain_event.h"
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
#include "key_mapper.h"


ClientGame::ClientGame(ConnectionHandler& connection, std::string& player_name, AudioManager& audio_manager,
                       FontManager& font_manager):
        sdl(SDL2pp::SDL(SDL_INIT_VIDEO)),
        window(SDL2pp::Window("Argentum Online", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_BORDERLESS)),
        renderer(SDL2pp::Renderer(window, -1, SDL_RENDERER_ACCELERATED)),
        connection(connection),
        player_name(player_name),
        world(renderer, connection.receive_map(), player_name, audio_manager, font_manager),
        key_being_pressed(SDLK_UNKNOWN),
        camera(initialize_world_and_camera()),
        ui(renderer, player_name, font_manager),
        keep_running(true),
        just_restored(false),
        is_chat_active(false),
        chat_text(""),
        cmd_handler(player_name, connection, chat_text, ui) {
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
    return {game_viewport.GetW(), game_viewport.GetH(), world_size, user};
}

void ClientGame::pollEvents() {
    SDL_Event event;
    bool key_was_pressed = false;

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

        if (is_chat_active) {
            handle_chat_events(event);
            continue;
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

    if (not keep_running) {
        connection.stop();
        return;
    }

    // TODO esto definitivamente habría que modularizarlo/encapsularlo
    if (!key_was_pressed && KeyMapper::is_movement_key(key_being_pressed)) {
        connection.push_command(std::make_unique<MoveEventDTO>(KeyMapper::get_direction(key_being_pressed)));
    }
}

void ClientGame::handle_chat_events(const SDL_Event& event) {
    if (event.type == SDL_TEXTINPUT) {
        chat_text += event.text.text;
        return;
    }

    if (event.type == SDL_KEYUP) {
        key_being_pressed = SDLK_UNKNOWN;
        return;
    }

    if (event.type != SDL_KEYDOWN)
        return;

    if (event.key.keysym.sym == SDLK_BACKSPACE && !chat_text.empty()) {
        chat_text.pop_back();
        return;
    }

    if (event.key.keysym.sym == SDLK_RETURN) {
        if (chat_text.empty())
            return;

        ui.chat_scroll_to_bottom();

        cmd_handler.process_chat_output();
    }

    if (event.key.keysym.sym == SDLK_ESCAPE) {
        is_chat_active = false;
        SDL_StopTextInput();
        chat_text.clear();
    }
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

    if (!KeyMapper::is_command_key(key_pressed))
        return;

    switch (key_pressed) {
        case SDLK_c:
            toggle_chat();
            break;
        case SDLK_e:
            cmd_handler.handle_pick_up_command();
            break;
        case SDLK_q:
            cmd_handler.handle_drop_item_command();
            break;
        default:
            throw std::runtime_error("Esta tecla aún no tiene una funcionalidad asignada");
    }
}

void ClientGame::handle_mouse_click(const SDL_Event& event) {
    assert(event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP);
    if (is_inside_viewport(event.button.x, event.button.y, game_viewport)) {
        handle_game_click(event);
    } else {
        handle_ui_click(event);
    }
}

void ClientGame::render_ui_and_world() {
    ui.render();

    ui.render_fields();

    ui.render_chat_history();

    ui.render_chat_input(chat_text, is_chat_active);

    renderer.SetViewport(game_viewport);
    world.render_in_z_order(camera);

    renderer.SetViewport(SDL2pp::NullOpt);

    // TODO actualizar campos de vida, mana en función a que se sabe del personaje, si escribió, etc.
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
        if (is_inside_viewport(x, y, chat_icon)) {
            toggle_chat();
        } else if (is_inside_viewport(x, y, minimize_button)) {
            window.Minimize();
        } else if (is_inside_viewport(x, y, close_button)) {
            keep_running = false;
        }
    }
}

void ClientGame::toggle_chat() {
    is_chat_active = !is_chat_active;

    if (is_chat_active) {
        SDL_StartTextInput();
    } else {
        SDL_StopTextInput();
    }
}

void ClientGame::handle_game_click(const SDL_Event& event) {
    const uint16_t tile_size = ClientConfig::get().get_tile_size();
    int game_click_x = event.button.x - game_viewport.x;
    int game_click_y = event.button.y - game_viewport.y;

    if (event.button.button == SDL_BUTTON_LEFT) {
        const uint16_t target_x = (camera.get_view().GetX() + game_click_x) / tile_size;
        const uint16_t target_y = (camera.get_view().GetY() + game_click_y) / tile_size;
        connection.push_command(std::make_unique<InteractEventDTO>(target_x, target_y));
    }
}

SDL_HitTestResult ClientGame::hit_test_callback(SDL_Window*, const SDL_Point* area, void* data) {
    ClientGame* game = static_cast<ClientGame*>(data);

    if (game->is_inside_viewport(area->x, area->y, game->header_bar)) {
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
