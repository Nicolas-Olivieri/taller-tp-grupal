#ifndef CLIENT_CONSTANTS_H
#define CLIENT_CONSTANTS_H

#define TILE_SIZE 32
#define FPS 30

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 726

#define WORLD_WIDTH 1200
#define WORLD_HEIGHT 1200

enum class Layer { BODY = 0, HEAD = 1, HELMET = 2, WEAPON = 3, SHIELD = 4 };

// TODO ir agregando según se agreguen categorias en los TOML
enum class SpriteCategory {
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
#endif  // CLIENT_CONSTANTS_H
