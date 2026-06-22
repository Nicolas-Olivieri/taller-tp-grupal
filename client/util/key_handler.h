#ifndef KEY_HANDLER_H
#define KEY_HANDLER_H


#include <string>

#include "client/SDL/user_interface.h"
#include "client/connection/connection_handler.h"

#include "command_handler.h"


class KeyHandler {

    ConnectionHandler& connection;
    UserInterface& ui;

    CommandHandler cmd_handler;

    int key_being_pressed;
    bool _is_chat_active;
    std::string chat_text;
    bool key_was_pressed;

public:
    KeyHandler(const std::string& player_name, ConnectionHandler& connection, UserInterface& ui);

    /// Principales

    void handle_key_down(const SDL_Event& event);

    void handle_chat_event(const SDL_Event& event);

    void handle_continuous_movement();

    /// Utilidades

    void toggle_chat();

    bool is_releasing_key(const SDL_Event& event);

    void release_key();

    /// Getters

    bool is_chat_active();

    const std::string& get_chat_text();

private:
    void close_chat();

    void handle_movement(const int key_pressed);

    void handle_command(const int key_pressed);
};


#endif  // KEY_HANDLER_H
