#ifndef CLIENT_CONSTANTS_H
#define CLIENT_CONSTANTS_H

#define NO_ITEM 0

enum class Layer { BODY = 0, HEAD = 1, HELMET = 2, WEAPON = 3, SHIELD = 4 };

enum class EffectType { DEATH = 0, MEDITATION = 1 };

enum class UiElement {
    CHAT = 0,
    INVENTORY = 1,
    SCREEN = 2,
    XP_BAR = 3,
    MANA_BAR = 4,
    HEALTH_BAR = 5,
    FOUNDER_CROWN = 6
};

#endif  // CLIENT_CONSTANTS_H
