#include "clan.h"

#include <format>
ClanActionResult Clan::execute(const ClanActionPayload& payload) {
    const std::string& player_name = payload.player_name;
    const std::string& target = payload.target;

    switch (payload.type) {
        case ClanActionType::ACCEPT:
            return accept(player_name, target);
        case ClanActionType::REJECT:
            return reject(player_name, target);
        case ClanActionType::KICK:
            return kick(player_name, target);
        case ClanActionType::BAN:
            return ban(player_name, target);
        case ClanActionType::LEAVE:
            return leave(player_name);
        case ClanActionType::REVIEW:
            return review(player_name);
        default:
            throw std::runtime_error(std::format("Al clan {}, {} le pidió una acción desconocida", clan_name,
                                                 payload.player_name));
    }
}
