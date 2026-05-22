
#include "snapshot_builder.h"

SnapshotDTO SnapshotBuilder::build() const { return SnapshotDTO(players_info, actions); }

void SnapshotBuilder::add_players(const std::unordered_map<std::string, Player>& players) {
    for (const auto& [player_name, player]: players) {
        players_info.push_back(convert_to_info(player_name, player));
    }
}

void SnapshotBuilder::add_action(const ActionDTO& action) { actions.push_back(action); }

PlayerInfoDTO SnapshotBuilder::convert_to_info(const std::string& player_name, const Player& player) {
    Position position = player.get_position();
    return PlayerInfoDTO(player_name, player.get_direction(), position.get_x(), position.get_y());
}
