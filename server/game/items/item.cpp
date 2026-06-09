#include "item.h"

#include <stdexcept>

#include "equipment.h"
#include "item_mapper.h"

Use::Use(Stats& stats, Equipment& equipment): stats(stats), equipment(equipment) {}

uint8_t Use::operator()(const Weapon& weapon) { return change_item(equipment.weapon, weapon.item); }

uint8_t Use::operator()(const Shield& shield) { return change_item(equipment.shield, shield.item); }

uint8_t Use::operator()(const Helmet& helmet) { return change_item(equipment.helmet, helmet.item); }

uint8_t Use::operator()(const Armor& armor) { return change_item(equipment.armor, armor.item); }

uint8_t Use::operator()(const Usable& usable) {
    UsableTypeEffect type = ItemMapper::get_usable_type_effect(usable.item);
    uint16_t amount = ItemMapper::get_usable_effect_amount(usable.item);

    switch (type) {
        case UsableTypeEffect::HEALTH:
            stats.health.recover(amount);
            break;
        case UsableTypeEffect::MANA:
            stats.mana.recover(amount);
            break;
        default:
            throw std::invalid_argument("Cannot use usable with an unknown type effect");
    }

    return NO_ITEM;
}

uint8_t Use::change_item(uint8_t& equipped, uint8_t new_item) {
    uint8_t old_item = equipped;
    equipped = new_item;

    return old_item;
}
