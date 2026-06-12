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
#include "sprites/sprite.h"

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
        chat_text("") {
    SDL_SetWindowHitTest(window.Get(), hit_test_callback, this);
}

void ClientGame::run() {

    RateTimer timer(FPS);
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
            world.update_players(info);
            break;
        }
    }
    Sprite& user = world.get_client_player();
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

        if (chat_text[0] == '@') {
            send_private_message();
        } else if (chat_text[0] == '/') {
            handle_text_command(chat_text);
        } else {
            connection.push_command(std::make_unique<ChatEventDTO>("", chat_text));
        }

        chat_text.clear();
    }

    if (event.key.keysym.sym == SDLK_ESCAPE) {
        is_chat_active = false;
        SDL_StopTextInput();
        chat_text.clear();
    }
}

void ClientGame::handle_text_command(const std::string& text) {
    assert(!text.empty());

    if (text == "/resucitar")
        connection.push_command(std::make_unique<EventDTO>(CommandType::RESURRECT));
    else if (text == "/curar")
        connection.push_command(std::make_unique<EventDTO>(CommandType::HEAL));
    else if (text == "/listar")
        connection.push_command(std::make_unique<EventDTO>(CommandType::LIST_ITEMS));
    else if (text == "/tomar")
        handle_pick_up_command();
    else if (text.starts_with("/comprar "))
        handle_buy_item_command(text);
    else if (text.starts_with("/vender "))
        handle_sell_item_command(text);

    else if (text.starts_with("/depositar oro "))
        handle_deposit_gold_command(text);
    else if (text.starts_with("/depositar "))
        handle_deposit_item_command(text);

    else if (text.starts_with("/retirar oro "))
        handle_withdraw_gold_command(text);
    else if (text.starts_with("/retirar "))
        handle_withdraw_item_command(text);

    else if (text == "/tirar")
        handle_drop_item_command();

    else if (text.starts_with("/fundar-clan "))
        handle_clan_foundation(text);
    else if (text.starts_with("/unirse "))
        handle_clan_join(text);
    else if (text.starts_with("/clan-"))
        handle_clan_operation(text);
    else if (text == "/revisar-clan")
        connection.push_command(std::make_unique<EventDTO>(CommandType::CLAN_REVIEW));
    else if (text == "/dejar-clan")
        connection.push_command(std::make_unique<EventDTO>(CommandType::CLAN_LEAVE));

    else if (text.starts_with("/cheat-"))
        handle_cheat(text);
}

void ClientGame::handle_pick_up_command() {
    connection.push_command(std::make_unique<EventDTO>(CommandType::PICKUP));
}

void ClientGame::handle_buy_item_command(const std::string& text) {
    std::optional<uint8_t> opt_item_id = ClientConfig::get().get_item_id(text.substr(9));
    if (opt_item_id.has_value()) {
        connection.push_command(std::make_unique<BuyEventDTO>(opt_item_id.value()));
    } else {
        std::vector<ActionDTO> local_error;
        local_error.push_back(ActionDTO(ChatMessageDTO(
                MessageType::ERROR, player_name, "Item desconocido. Escribi /listar para ver el catalogo.")));
        ui.update_chat(local_error);
    }
}

void ClientGame::handle_sell_item_command(const std::string& text) {
    std::optional<uint8_t> opt_item_id = ClientConfig::get().get_item_id(text.substr(8));
    if (opt_item_id.has_value()) {
        connection.push_command(std::make_unique<SellEventDTO>(opt_item_id.value()));
    } else {
        std::vector<ActionDTO> local_error;
        local_error.push_back(
                ActionDTO(ChatMessageDTO(MessageType::ERROR, player_name, "Item desconocido.")));
        ui.update_chat(local_error);
    }
}

void ClientGame::handle_deposit_gold_command(const std::string& text) {
    std::string amount = text.substr(15);

    if (amount.empty() || not std::ranges::all_of(amount, ::isdigit)) {
        std::vector<ActionDTO> local_error;
        local_error.push_back(
                ActionDTO(ChatMessageDTO(MessageType::ERROR, player_name,
                                         "La cantidad ingresada debe ser un numero entero positivo")));
        ui.update_chat(local_error);
        return;
    }

    try {
        const auto gold_amount = std::stoul(amount);
        if (gold_amount >= UINT16_MAX) {
            throw std::out_of_range("Cantidad de oro fuera de los limites permitidos.");
        }

        connection.push_command(std::make_unique<DepositGoldEventDTO>(static_cast<uint16_t>(gold_amount)));

    } catch (const std::out_of_range&) {
        std::vector<ActionDTO> local_error;
        local_error.push_back(ActionDTO(ChatMessageDTO(
                MessageType::ERROR, player_name, "La cantidad ingresada supera el limite de la boveda")));
        ui.update_chat(local_error);
    }
}

void ClientGame::handle_deposit_item_command(const std::string& text) {
    std::optional<uint8_t> opt_item_id = ClientConfig::get().get_item_id(text.substr(11));
    if (opt_item_id.has_value()) {
        connection.push_command(std::make_unique<DepositItemEventDTO>(opt_item_id.value()));

    } else {
        std::vector<ActionDTO> local_error;
        local_error.push_back(ActionDTO(ChatMessageDTO(MessageType::ERROR, player_name, "Item desconocido")));
        ui.update_chat(local_error);
    }
}

void ClientGame::handle_withdraw_gold_command(const std::string& text) {
    std::string amount = text.substr(13);

    if (amount.empty() || not std::ranges::all_of(amount, ::isdigit)) {
        std::vector<ActionDTO> local_error;
        local_error.push_back(
                ActionDTO(ChatMessageDTO(MessageType::ERROR, player_name,
                                         "La cantidad ingresada debe ser un numero entero positivo")));
        ui.update_chat(local_error);
        return;
    }

    try {
        const auto gold_amount = std::stoul(amount);
        if (gold_amount >= UINT16_MAX) {
            throw std::out_of_range("Cantidad de oro fuera de los limites permitidos.");
        }

        connection.push_command(std::make_unique<WithdrawGoldEventDTO>(static_cast<uint16_t>(gold_amount)));

    } catch (const std::out_of_range&) {
        std::vector<ActionDTO> local_error;
        local_error.push_back(ActionDTO(ChatMessageDTO(
                MessageType::ERROR, player_name, "La cantidad ingresada supera el limite de la boveda")));
        ui.update_chat(local_error);
    }
}

void ClientGame::handle_withdraw_item_command(const std::string& text) {
    std::optional<uint8_t> opt_item_id = ClientConfig::get().get_item_id(text.substr(9));
    if (opt_item_id.has_value()) {
        connection.push_command(std::make_unique<WithdrawItemEventDTO>(opt_item_id.value()));

    } else {
        std::vector<ActionDTO> local_error;
        local_error.push_back(ActionDTO(ChatMessageDTO(MessageType::ERROR, player_name, "Item desconocido")));
        ui.update_chat(local_error);
    }
}

void ClientGame::handle_drop_item_command() {
    auto bound_id = ui.get_bound_item_id();
    if (bound_id.has_value()) {
        connection.push_command(std::make_unique<DropItemEventDTO>(bound_id.value()));
        ui.clear_bound_item();

    } else {
        std::vector<ActionDTO> local_error;
        local_error.push_back(ActionDTO(ChatMessageDTO(MessageType::ERROR, player_name,
                                                       "No tenes ningun item seleccionado para tirar")));
        ui.update_chat(local_error);
    }
}

void ClientGame::send_private_message() {
    size_t first_space = chat_text.find(' ');

    if (first_space != std::string::npos) {
        std::string nick = chat_text.substr(1, first_space - 1);
        std::string message = chat_text.substr(first_space + 1);
        connection.push_command(std::make_unique<ChatEventDTO>(nick, message));
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
            handle_pick_up_command();
            break;
        case SDLK_q:
            handle_drop_item_command();
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
    int game_click_x = event.button.x - game_viewport.x;
    int game_click_y = event.button.y - game_viewport.y;

    if (event.button.button == SDL_BUTTON_LEFT) {
        const uint16_t target_x = (camera.get_view().GetX() + game_click_x) / TILE_SIZE;
        const uint16_t target_y = (camera.get_view().GetY() + game_click_y) / TILE_SIZE;
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

void ClientGame::handle_clan_foundation(const std::string& text) {
    const std::string prefix = "/fundar-clan ";

    std::string clan_name = extract_prefix(prefix, text);

    if (clan_name.empty())
        return;

    trim_text(clan_name);

    connection.push_command(std::make_unique<ClanFoundEventDTO>(clan_name));
}

void ClientGame::handle_clan_join(const std::string& text) {
    const std::string prefix = "/unirse ";

    std::string clan_name = extract_prefix(prefix, text);

    if (clan_name.empty())
        return;

    trim_text(clan_name);

    connection.push_command(std::make_unique<ClanJoinEventDTO>(clan_name));
}

std::string ClientGame::extract_prefix(const std::string& prefix, const std::string& text) const {
    assert(not prefix.empty());
    assert(not text.empty());
    assert(text.starts_with(prefix));
    size_t pos = text.find(prefix.back());
    assert(pos != std::string::npos);

    std::string clan_name = text.substr(pos + 1);
    if (clan_name.empty())
        return clan_name;

    return clan_name;
}

void ClientGame::trim_text(std::string& text) {
    auto not_space = [](uint8_t c) { return !std::isspace(c); };
    text.erase(text.begin(), std::find_if(text.begin(), text.end(), not_space));
    text.erase(std::find_if(text.rbegin(), text.rend(), not_space).base(), text.end());
}

void ClientGame::handle_clan_operation(const std::string& text) {
    const std::string prefix = "/clan-";

    std::string operation = extract_prefix(prefix, text);

    if (operation.empty())
        return;

    if (operation.starts_with("aceptar "))
        handle_clan_accept(operation);
    if (operation.starts_with("rechazar "))
        handle_clan_reject(operation);
    if (operation.starts_with("kick "))
        handle_clan_kick(operation);
    if (operation.starts_with("ban "))
        handle_clan_ban(operation);
}

void ClientGame::handle_clan_accept(const std::string& text) {
    const std::string prefix = "aceptar ";

    std::string other_player = extract_prefix(prefix, text);

    connection.push_command(std::make_unique<RequestResponseEventDTO>(other_player, true));
}

void ClientGame::handle_clan_reject(const std::string& text) {
    const std::string prefix = "rechazar ";

    std::string other_player = extract_prefix(prefix, text);

    connection.push_command(std::make_unique<RequestResponseEventDTO>(other_player, false));
}

void ClientGame::handle_clan_kick(const std::string& text) {
    const std::string prefix = "kick ";

    std::string other_player = extract_prefix(prefix, text);

    connection.push_command(std::make_unique<ClanRemovePlayerEventDTO>(other_player, false));
}

void ClientGame::handle_clan_ban(const std::string& text) {
    const std::string prefix = "ban ";

    std::string other_player = extract_prefix(prefix, text);

    connection.push_command(std::make_unique<ClanRemovePlayerEventDTO>(other_player, true));
}

void ClientGame::handle_cheat(const std::string& text) {
    const std::string prefix = "/cheat-";

    std::string cheat_type = extract_prefix(prefix, text);

    if (cheat_type.starts_with("set-xp "))
        handle_xp_cheat(cheat_type);
    else if (cheat_type.starts_with("gain-gold "))
        handle_gold_cheat(cheat_type);
    else if (cheat_type == "kill-self")
        handle_kill_self_cheat();

    // TODO agregar el resto de cheats
}

void ClientGame::handle_xp_cheat(const std::string& text) {
    const std::string prefix = "set-xp ";

    const std::string level_txt = extract_prefix(prefix, text);

    std::regex only_numbers("^\\d+$");

    if (!std::regex_match(level_txt, only_numbers))
        return;

    uint8_t level;
    auto [_, error_code] = std::from_chars(level_txt.data(), level_txt.data() + level_txt.size(), level);

    if (error_code != std::errc())
        return;

    connection.push_command(std::make_unique<CheatExperienceSetEventDTO>(level));
}

void ClientGame::handle_gold_cheat(const std::string& text) {
    const std::string prefix = "gain-gold ";

    const std::string amount_text = extract_prefix(prefix, text);

    std::regex only_numbers("^\\d+$");
    if (!std::regex_match(amount_text, only_numbers))
        return;

    uint16_t gold_amount;
    auto [_, error_code] = std::from_chars<uint16_t>(amount_text.data(),
                                                     amount_text.data() + amount_text.size(), gold_amount);

    if (error_code != std::errc())
        return;

    connection.push_command(std::make_unique<CheatGoldGainEventDTO>(gold_amount));
}

void ClientGame::handle_kill_self_cheat() {
    connection.push_command(std::make_unique<EventDTO>(CommandType::CHEAT_DEATH));
}
