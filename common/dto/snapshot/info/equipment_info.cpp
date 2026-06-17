#include "equipment_info.h"

#include "common/protocol/serializer.h"


EquipmentInfoDTO::EquipmentInfoDTO() {}


EquipmentInfoDTO::EquipmentInfoDTO(const EquipableItemInfoDTO& weapon, const EquipableItemInfoDTO& shield,
                                   const EquipableItemInfoDTO& helmet, const EquipableItemInfoDTO& armor):
        weapon(weapon), shield(shield), helmet(helmet), armor(armor) {}


size_t EquipmentInfoDTO::message_size() const {
    return weapon.message_size() + shield.message_size() + helmet.message_size() + armor.message_size();
}


void EquipmentInfoDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
