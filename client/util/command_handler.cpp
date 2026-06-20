#include "command_handler.h"

#include <cassert>
#include <memory>
#include <regex>
#include <vector>

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

CommandHandler::CommandHandler(const std::string& player_name, ConnectionHandler& connection,
                               std::string& chat_text, UserInterface& ui):
        player_name(player_name), connection(connection), chat_text(chat_text), ui(ui) {}

void CommandHandler::process_chat_output() {
    assert(not chat_text.empty());

    switch (chat_text[0]) {
        case '@':
            handle_private_message();
            break;
        case '/':
            handle_text_command();
            break;
        default:
            connection.push_command(std::make_unique<ChatEventDTO>("", chat_text));
            break;
    }

    chat_text.clear();
    assert(chat_text.empty());
}

void CommandHandler::handle_private_message() {
    assert(not chat_text.empty());
    assert(chat_text[0] == '@');

    size_t first_space = chat_text.find(' ');

    if (first_space != std::string::npos) {
        std::string nick = chat_text.substr(1, first_space - 1);
        std::string message = chat_text.substr(first_space + 1);
        connection.push_command(std::make_unique<ChatEventDTO>(nick, message));
    }
}

void CommandHandler::handle_text_command() {
    assert(not chat_text.empty());
    assert(chat_text[0] == '/');

    if (chat_text == "/ayuda")
        ui.toggle_help();
    else if (chat_text == "/resucitar")
        connection.push_command(std::make_unique<EventDTO>(CommandType::RESURRECT));
    else if (chat_text == "/curar")
        connection.push_command(std::make_unique<EventDTO>(CommandType::HEAL));
    else if (chat_text == "/listar")
        connection.push_command(std::make_unique<EventDTO>(CommandType::LIST_ITEMS));
    else if (chat_text == "/tomar")
        handle_pick_up_command();

    else if (chat_text.starts_with("/comprar "))
        handle_buy_item_command(chat_text);

    else if (chat_text.starts_with("/vender "))
        handle_sell_item_command(chat_text);

    else if (chat_text.starts_with("/depositar oro "))
        handle_deposit_gold_command(chat_text);
    else if (chat_text.starts_with("/depositar "))
        handle_deposit_item_command(chat_text);

    else if (chat_text.starts_with("/retirar oro "))
        handle_withdraw_gold_command(chat_text);
    else if (chat_text.starts_with("/retirar "))
        handle_withdraw_item_command(chat_text);

    else if (chat_text == "/tirar")
        handle_drop_item_command();

    else if (chat_text.starts_with("/fundar-clan "))
        handle_clan_foundation(chat_text);

    else if (chat_text.starts_with("/unirse "))
        handle_clan_join(chat_text);

    else if (chat_text.starts_with("/clan-"))
        handle_clan_operation(chat_text);

    else if (chat_text == "/revisar-clan")
        connection.push_command(std::make_unique<EventDTO>(CommandType::CLAN_REVIEW));

    else if (chat_text == "/dejar-clan")
        connection.push_command(std::make_unique<EventDTO>(CommandType::CLAN_LEAVE));

    else if (chat_text.starts_with("/cheat-"))
        handle_cheat(chat_text);

    else if (chat_text.starts_with("/meditar"))
        handle_meditate();

    else if (chat_text == "/viajar")
        handle_teleport();

    else if (chat_text == "/inventario")
        handle_inventory_request();
}

void CommandHandler::handle_pick_up_command() {
    connection.push_command(std::make_unique<EventDTO>(CommandType::PICKUP));
}

void CommandHandler::handle_drop_item_command() {
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

/// Handlers específicos

void CommandHandler::handle_buy_item_command(const std::string& text) {
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

void CommandHandler::handle_sell_item_command(const std::string& text) {
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

void CommandHandler::handle_deposit_gold_command(const std::string& text) {
    const std::string prefix = "/depositar oro ";
    std::string amount = text.substr(prefix.size());

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

void CommandHandler::handle_deposit_item_command(const std::string& text) {
    std::optional<uint8_t> opt_item_id = ClientConfig::get().get_item_id(text.substr(11));
    if (opt_item_id.has_value()) {
        connection.push_command(std::make_unique<DepositItemEventDTO>(opt_item_id.value()));

    } else {
        std::vector<ActionDTO> local_error;
        local_error.push_back(ActionDTO(ChatMessageDTO(MessageType::ERROR, player_name, "Item desconocido")));
        ui.update_chat(local_error);
    }
}

void CommandHandler::handle_withdraw_gold_command(const std::string& text) {
    const std::string prefix = "/retirar oro ";
    std::string amount = text.substr(prefix.size());

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

void CommandHandler::handle_withdraw_item_command(const std::string& text) {
    std::optional<uint8_t> opt_item_id = ClientConfig::get().get_item_id(text.substr(9));
    if (opt_item_id.has_value()) {
        connection.push_command(std::make_unique<WithdrawItemEventDTO>(opt_item_id.value()));

    } else {
        std::vector<ActionDTO> local_error;
        local_error.push_back(ActionDTO(ChatMessageDTO(MessageType::ERROR, player_name, "Item desconocido")));
        ui.update_chat(local_error);
    }
}

void CommandHandler::handle_meditate() {
    connection.push_command(std::make_unique<EventDTO>(CommandType::MEDITATE));
}

void CommandHandler::handle_teleport() {
    connection.push_command(std::make_unique<EventDTO>(CommandType::TELEPORT));
}

/// Handlers de clanes

void CommandHandler::handle_clan_foundation(const std::string& text) {
    const std::string prefix = "/fundar-clan ";

    std::string clan_name = extract_prefix(prefix, text);

    if (clan_name.empty())
        return;

    trim_text(clan_name);

    connection.push_command(std::make_unique<ClanFoundEventDTO>(clan_name));
}

void CommandHandler::handle_clan_join(const std::string& text) {
    const std::string prefix = "/unirse ";

    std::string clan_name = extract_prefix(prefix, text);

    if (clan_name.empty())
        return;

    trim_text(clan_name);

    connection.push_command(std::make_unique<ClanJoinEventDTO>(clan_name));
}

void CommandHandler::handle_clan_operation(const std::string& text) {
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

void CommandHandler::handle_clan_accept(const std::string& text) {
    const std::string prefix = "aceptar ";

    std::string other_player = extract_prefix(prefix, text);

    connection.push_command(std::make_unique<RequestResponseEventDTO>(other_player, true));
}

void CommandHandler::handle_clan_reject(const std::string& text) {
    const std::string prefix = "rechazar ";

    std::string other_player = extract_prefix(prefix, text);

    connection.push_command(std::make_unique<RequestResponseEventDTO>(other_player, false));
}

void CommandHandler::handle_clan_kick(const std::string& text) {
    const std::string prefix = "kick ";

    std::string other_player = extract_prefix(prefix, text);

    connection.push_command(std::make_unique<ClanRemovePlayerEventDTO>(other_player, false));
}

void CommandHandler::handle_clan_ban(const std::string& text) {
    const std::string prefix = "ban ";

    std::string other_player = extract_prefix(prefix, text);

    connection.push_command(std::make_unique<ClanRemovePlayerEventDTO>(other_player, true));
}

/// Handlers de cheats

void CommandHandler::handle_cheat(const std::string& text) {
    const std::string prefix = "/cheat-";

    std::string cheat_type = extract_prefix(prefix, text);

    if (cheat_type.starts_with("set-xp "))
        handle_xp_cheat(cheat_type);
    else if (cheat_type.starts_with("gain-gold "))
        handle_gold_cheat(cheat_type);
    else if (cheat_type == "kill-self")
        handle_kill_self_cheat();
    else if (cheat_type == "infinite-recoverables")
        handle_infinite_recoverables_cheat();
    else if (cheat_type.starts_with("get-item "))
        handle_get_item_cheat(cheat_type);
    else if (cheat_type == "kill-creatures")
        handle_kill_creatures_cheat();

    // TODO agregar el resto de cheats
}

void CommandHandler::handle_xp_cheat(const std::string& text) {
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

void CommandHandler::handle_gold_cheat(const std::string& text) {
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

void CommandHandler::handle_kill_self_cheat() {
    connection.push_command(std::make_unique<EventDTO>(CommandType::CHEAT_DEATH));
}

void CommandHandler::handle_infinite_recoverables_cheat() {
    connection.push_command(std::make_unique<EventDTO>(CommandType::CHEAT_INFINITE_RECOVERABLES));
}

void CommandHandler::handle_get_item_cheat(const std::string& text) {
    const std::string prefix = "get-item ";

    const std::string item_text = extract_prefix(prefix, text);

    std::optional<uint8_t> item_id = ClientConfig::get().get_item_id(item_text);
    if (!item_id.has_value())
        return;

    connection.push_command(std::make_unique<CheatGetItemEventDTO>(item_id.value()));
}

void CommandHandler::handle_kill_creatures_cheat() {
    connection.push_command(std::make_unique<EventDTO>(CommandType::CHEAT_KILL_CREATURES));
}

/// Auxiliares

std::string CommandHandler::extract_prefix(const std::string& prefix, const std::string& text) const {
    assert(not prefix.empty());
    assert(not text.empty());
    assert(text.starts_with(prefix));
    size_t pos = text.find(prefix.back());
    assert(pos != std::string::npos);

    std::string remainder = text.substr(pos + 1);
    if (remainder.empty())
        return remainder;

    return remainder;
}

void CommandHandler::trim_text(std::string& text) {
    auto not_space = [](uint8_t c) { return !std::isspace(c); };
    text.erase(text.begin(), std::find_if(text.begin(), text.end(), not_space));
    text.erase(std::find_if(text.rbegin(), text.rend(), not_space).base(), text.end());
}

void CommandHandler::handle_inventory_request() {
    connection.push_command(std::make_unique<EventDTO>(CommandType::INVENTORY_INFO));
}
