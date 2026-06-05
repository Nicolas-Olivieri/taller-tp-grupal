
#ifndef SNAPSHOT_BUILDER_H
#define SNAPSHOT_BUILDER_H

#include <string>
#include <unordered_map>
#include <vector>

#include "server/game/creatures/creature.h"
#include "server/game/player/player.h"

#include "snapshot.h"


class SnapshotBuilder {

    std::vector<PlayerInfoDTO> players_info;
    std::vector<CreatureInfoDTO> creatures_info;
    std::vector<ActionDTO> actions;

public:
    SnapshotBuilder() = default;

    SnapshotDTO build() const;

    void add_players(const std::unordered_map<std::string, Player>& players);

    void add_creatures(const std::unordered_map<uint16_t, Creature>& creatures);

    void add_action(const ActionDTO& action);

    // Elimina el constructor y el operador de asignación por copia
    SnapshotBuilder(const SnapshotBuilder&) = delete;
    SnapshotBuilder& operator=(const SnapshotBuilder&) = delete;
    SnapshotBuilder(SnapshotBuilder&&) = delete;
    SnapshotBuilder& operator=(SnapshotBuilder&&) = delete;

private:
    PlayerInfoDTO convert_to_info(const std::string& player_name, const Player& player);

    CreatureInfoDTO convert_to_info(uint16_t sub_id, const Creature& creature);
};


#endif  // SNAPSHOT_BUILDER_H
