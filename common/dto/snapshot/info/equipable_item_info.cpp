#include "equipable_item_info.h"

#include "common/protocol/serializer.h"


EquipableItemInfoDTO::EquipableItemInfoDTO(): item_id(0), effect(0) {}


EquipableItemInfoDTO::EquipableItemInfoDTO(const uint8_t item_id, const uint8_t effect):
        item_id(item_id), effect(effect) {}


size_t EquipableItemInfoDTO::message_size() const { return sizeof(uint8_t) + sizeof(uint8_t); }


void EquipableItemInfoDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
