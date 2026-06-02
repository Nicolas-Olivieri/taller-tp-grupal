#ifndef PLAYERINFODTO_H
#define PLAYERINFODTO_H

#include <string>

#include "common/direction.h"
#include "common/dto/message.h"

#include "appearance.h"
#include "player_stats.h"

struct PlayerInfoDTO: public ProtocolMessageDTO {
    std::string name;
    Direction direction;
    uint16_t x;
    uint16_t y;
    AppearanceDTO appearance;
    PlayerStatsDTO stats;
    // PlayersStatsDTO stats; TODO: agregar esto Y NO OLVIDAR MODIFICAR LOS
    // MÉTODOS PARA TENERLO EN CUENTA

    PlayerInfoDTO(const std::string& name, const Direction& direction, uint16_t x, uint16_t y,
                  const AppearanceDTO& appearance, const PlayerStatsDTO& stats);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};

#endif  // PLAYERINFODTO_H
