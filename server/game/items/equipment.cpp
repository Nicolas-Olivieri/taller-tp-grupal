#include "equipment.h"

#include <cassert>

Unequip::Unequip(Equipment& equipment): equipment(equipment) {}

bool Unequip::operator()(const Weapon& weapon) { return unequip_item(equipment.weapon, weapon.item); }

bool Unequip::operator()(const Shield& shield) { return unequip_item(equipment.shield, shield.item); }

bool Unequip::operator()(const Helmet& helmet) { return unequip_item(equipment.helmet, helmet.item); }

bool Unequip::operator()(const Armor& armor) { return unequip_item(equipment.armor, armor.item); }

bool Unequip::operator()(const Usable& /* usable */) { return false; }

bool Unequip::unequip_item(uint8_t& equipped, uint8_t item) {
    if (equipped != item || item == NO_ITEM)
        return false;

    equipped = NO_ITEM;
    return true;
}
