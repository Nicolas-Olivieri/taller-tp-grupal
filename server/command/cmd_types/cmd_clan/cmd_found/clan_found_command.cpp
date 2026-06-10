#include "clan_found_command.h"

#include <format>

#include "server/game/clan/clan.h"

ClanFoundCommand::ClanFoundCommand(const std::string& player_name, const std::string& clan_name):
        player_name(player_name), clan_name(clan_name), result(FoundClanResult::NO_RESULT) {}

void ClanFoundCommand::execute(GameWorld& world) { result = world.found_clan(player_name, clan_name); }

void ClanFoundCommand::build_snapshot(SnapshotBuilder& builder) {
    std::string error_msg;
    switch (result) {
        case FoundClanResult::SUCCESS:
            builder.add_action(ActionDTO(ChatMessageDTO(
                    MessageType::CLAN, player_name, std::format("Has fundado el clan: \"{}\"", clan_name))));
            return;

        case FoundClanResult::NOT_ENOUGH_LEVEL:
            error_msg = std::format("Debes tener nivel {} como minimo para fundar tu propio clan",
                                    Clan::MIN_LEVEL_REQUIRED_TO_FOUND_CLAN);
            break;
        case FoundClanResult::ALREADY_IN_CLAN:
            error_msg = "Ya perteneces a un clan. Debes abandonarlo para poder fundar otro";
            break;
        case FoundClanResult::CLAN_ALREADY_EXISTS:
            error_msg = std::format("Ya existe el clan \"{}\", prueba con otro nombre", clan_name);
            break;
        case FoundClanResult::CLAN_NAME_LONG:
            error_msg = std::format("El nombre de clan \"{}\" excede los {} caracteres", clan_name, CLAN_NAME);
            break;
        case FoundClanResult::NO_RESULT:
        default:
            throw std::runtime_error("ClanFoundCommand encontró un tipo de resultado inválido");
    }
    builder.add_action(ActionDTO(ChatMessageDTO(MessageType::ERROR, player_name, error_msg)));
}
