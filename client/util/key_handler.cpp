#include "key_handler.h"

#include <memory>

#include "common/dto/events/movement/moveevent.h"

#include "key_mapper.h"

KeyHandler::KeyHandler(const std::string& player_name, ConnectionHandler& connection, UserInterface& ui):
        connection(connection),
        ui(ui),
        cmd_handler(player_name, connection, chat_text, ui),
        key_being_pressed(SDLK_UNKNOWN),
        _is_chat_active(false),
        chat_text(""),
        key_was_pressed(false) {}

void KeyHandler::handle_continuous_movement() {
    if (!key_was_pressed && KeyMapper::is_movement_key(key_being_pressed)) {
        connection.push_command(std::make_unique<MoveEventDTO>(KeyMapper::get_direction(key_being_pressed)));
    }

    key_was_pressed = false;
}

bool KeyHandler::is_releasing_key(const SDL_Event& event) {
    return event.type == SDL_KEYUP && KeyMapper::is_movement_key(event.key.keysym.sym);
}

void KeyHandler::release_key() { key_being_pressed = SDLK_UNKNOWN; }

void KeyHandler::toggle_chat() {
    _is_chat_active = !_is_chat_active;

    if (_is_chat_active) {
        SDL_StartTextInput();
    } else {
        SDL_StopTextInput();
    }
}

void KeyHandler::close_chat() {
    _is_chat_active = false;
    SDL_StopTextInput();
    chat_text.clear();
}

bool KeyHandler::is_chat_active() { return _is_chat_active; }

void KeyHandler::handle_key_down(const SDL_Event& event) {
    assert(event.type == SDL_KEYDOWN);
    auto key_pressed = event.key.keysym.sym;
    key_was_pressed = true;

    handle_movement(key_pressed);
    handle_command(key_pressed);
}

void KeyHandler::handle_chat_event(const SDL_Event& event) {
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
        close_chat();
    }
}

void KeyHandler::handle_movement(const int key_pressed) {
    if (!KeyMapper::is_movement_key(key_pressed))
        return;

    Direction direction_chosen = KeyMapper::get_direction(key_pressed);

    connection.push_command(std::make_unique<MoveEventDTO>(MoveEventDTO(direction_chosen)));
    key_being_pressed = key_pressed;
}

void KeyHandler::handle_command(const int key_pressed) {
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
        case SDLK_h:
            ui.toggle_help(HelpPage::GENERAL);
            break;
        case SDLK_m:
            cmd_handler.handle_meditate();
            break;
        default:
            throw std::runtime_error("Esta tecla aún no tiene una funcionalidad asignada");
    }
}
const std::string& KeyHandler::get_chat_text() { return chat_text; }
