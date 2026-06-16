#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H


#include <string>

#include "client/SDL/user_interface.h"
#include "client/connection/connection_handler.h"

class CommandHandler {

    const std::string& player_name;
    ConnectionHandler& connection;
    std::string& chat_text;
    UserInterface& ui;

public:
    CommandHandler(const std::string& player_name, ConnectionHandler& connection, std::string& chat_text,
                   UserInterface& ui);

    void process_chat_output();

    /// Handlers públicos, pues pueden accionarse con teclas

    void handle_pick_up_command();

    void handle_drop_item_command();

private:
    /// Handlers generales

    void handle_private_message();

    void handle_text_command();

    /// Handlers específicos

    void handle_buy_item_command(const std::string& text);

    void handle_sell_item_command(const std::string& text);

    void handle_deposit_gold_command(const std::string& text);

    void handle_deposit_item_command(const std::string& text);

    void handle_withdraw_gold_command(const std::string& text);

    void handle_withdraw_item_command(const std::string& text);

    void handle_meditate() const;

    /// Handlers de clanes

    void handle_clan_foundation(const std::string& text);

    void handle_clan_join(const std::string& text);

    void handle_clan_operation(const std::string& text);

    void handle_clan_accept(const std::string& text);

    void handle_clan_reject(const std::string& text);

    void handle_clan_kick(const std::string& text);

    void handle_clan_ban(const std::string& text);

    /// Handlers de cheats

    void handle_cheat(const std::string& text);

    void handle_xp_cheat(const std::string& text);

    void handle_gold_cheat(const std::string& text);

    void handle_kill_self_cheat();

    void handle_infinite_recoverables_cheat();

    void handle_get_item_cheat(const std::string& text);

    void handle_kill_creatures_cheat();

    /// Auxiliares

    std::string extract_prefix(const std::string& prefix, const std::string& text) const;

    void trim_text(std::string& text);
};


#endif  // COMMAND_HANDLER_H
