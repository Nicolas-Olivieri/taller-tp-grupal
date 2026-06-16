#ifndef TELEPORTATION_TOTEM_H
#define TELEPORTATION_TOTEM_H


#include "ally.h"

class TeleportationTotem: public Ally {
    Position peer_position;

public:
    explicit TeleportationTotem(const Position& position, const Position& peer_position);

    AllyExecuteResult execute(Player& player, const AllyActionPayload& payload) const override;

    ~TeleportationTotem() override = default;

private:
    //    AllyExecuteResult handle_tp(Player& player) const;
};


#endif  // TELEPORTATION_TOTEM_H
