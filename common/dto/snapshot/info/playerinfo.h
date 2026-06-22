#ifndef PLAYERINFODTO_H
#define PLAYERINFODTO_H

#include <string>

#include "common/direction.h"
#include "common/dto/message.h"

#include "appearance.h"
#include "clan_info.h"
#include "equipment_info.h"
#include "inventory_info.h"
#include "player_stats.h"

struct PlayerInfoDTO: public ProtocolMessageDTO {
    std::string name;
    ClanInfoDTO clan;
    Direction direction;
    uint16_t x;
    uint16_t y;
    uint16_t safe_gold;
    uint16_t excess_gold;
    AppearanceDTO appearance;
    PlayerStatsDTO stats;
    InventoryInfoDTO inventory;
    EquipmentInfoDTO equipment;

    PlayerInfoDTO(const std::string& name, const ClanInfoDTO& clan, const Direction& direction, uint16_t x,
                  uint16_t y, uint16_t safe_gold, uint16_t excess_gold, const AppearanceDTO& appearance,
                  const PlayerStatsDTO& stats, const InventoryInfoDTO& inventory,
                  const EquipmentInfoDTO& equipment);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};

#endif  // PLAYERINFODTO_H
