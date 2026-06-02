
#ifndef SNAPSHOT_BUILDER_H
#define SNAPSHOT_BUILDER_H

#include <string>
#include <unordered_map>
#include <vector>

#include "server/game/player/player.h"

#include "snapshot.h"


class SnapshotBuilder {

    std::vector<PlayerInfoDTO> players_info;
    std::vector<ActionDTO> actions;

public:
    SnapshotBuilder() = default;

    SnapshotDTO build() const;

    void add_players(const std::unordered_map<std::string, Player>& players);

    void add_action(const ActionDTO& action);

    // Elimina el constructor y el operador de asignación por copia
    SnapshotBuilder(const SnapshotBuilder&) = delete;
    SnapshotBuilder& operator=(const SnapshotBuilder&) = delete;
    SnapshotBuilder(SnapshotBuilder&&) = delete;
    SnapshotBuilder& operator=(SnapshotBuilder&&) = delete;

private:
    PlayerInfoDTO convert_to_info(const std::string& player_name, const Player& player);
};


#endif  // SNAPSHOT_BUILDER_H
