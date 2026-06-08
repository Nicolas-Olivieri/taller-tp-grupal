#include "equipment_info.h"

#include "common/protocol/serializer.h"


EquipmentInfoDTO::EquipmentInfoDTO(): weapon(0), shield(0), helmet(0), armor(0) {}


EquipmentInfoDTO::EquipmentInfoDTO(const uint8_t weapon, const uint8_t shield, const uint8_t helmet,
                                   const uint8_t armor):
        weapon(weapon), shield(shield), helmet(helmet), armor(armor) {}


size_t EquipmentInfoDTO::message_size() const {
    return sizeof(weapon) + sizeof(shield) + sizeof(helmet) + sizeof(armor);
}


void EquipmentInfoDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
