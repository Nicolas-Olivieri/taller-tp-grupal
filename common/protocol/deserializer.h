#ifndef DESERIALIZER_H
#define DESERIALIZER_H

#include <cstdint>
#include <string>
#include <vector>

#include "../dto/lobby/ally_info.h"
#include "common/direction.h"
#include "common/dto/events/command.h"
#include "common/dto/snapshot/actions/action.h"
#include "common/dto/snapshot/info/playerinfo.h"
#include "common/socket.h"

class Deserializer {
private:
    Socket& socket;

    PlayerInfoDTO recv_player_info();

    AllyInfoDTO recv_ally_info();

    AllyType recv_ally_type();

    ActionDTO recv_action();

    ActionType recv_action_type();

    MessageVisibility recv_message_visibility();

    AppearanceDTO recv_appearance();

    DespawnDTO recv_despawn();

    ChatMessageDTO recv_chat_message();

public:
    explicit Deserializer(Socket& socket);  // NOLINT

    std::string recv_string();

    uint8_t recv_uint8();

    uint16_t recv_uint16();

    CommandType recv_command_type();

    Direction recv_direction();

    std::vector<PlayerInfoDTO> recv_players_information();

    std::vector<ActionDTO> recv_actions();

    std::vector<AllyInfoDTO> recv_allies_information();
};

#endif  // DESERIALIZER_H
