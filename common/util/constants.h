#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <cstdint>

// TODO ir agregando según se agreguen categorias en los TOML
enum class SpriteCategory : uint8_t {
    GOBLIN = 0,
    SKELETON = 1,
    ZOMBIE = 2,
    SPIDER = 3,
    ORC = 4,
    GOLEM = 5,
    HEAD,
    BODY,

    TILE,
    COLLIDER,
    NPC
};

#endif  // CONSTANTS_H
