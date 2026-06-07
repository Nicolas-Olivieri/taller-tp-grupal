#ifndef ITEM_H
#define ITEM_H

#include <cstdint>
#include <variant>

#include "server/game/stats/stats.h"

struct Equipment;

enum class UsableTypeEffect : uint8_t { HEALTH = 0, MANA = 1 };

struct Weapon {
    uint8_t item;
};
struct Shield {
    uint8_t item;
};
struct Helmet {
    uint8_t item;
};
struct Armor {
    uint8_t item;
};
struct Usable {
    uint8_t item;
};

using Item = std::variant<Weapon, Shield, Helmet, Armor, Usable>;

// Functor que define cómo se usa cada tipo de Item
class Use {
private:
    Stats& stats;
    Equipment& equipment;

    uint8_t change_item(uint8_t& equipped, uint8_t new_item);

public:
    Use(Stats& stats, Equipment& equipment);

    uint8_t operator()(const Weapon& weapon);
    uint8_t operator()(const Shield& shield);
    uint8_t operator()(const Helmet& helmet);
    uint8_t operator()(const Armor& armor);
    uint8_t operator()(const Usable& usable);
};

#endif  // ITEM_H
