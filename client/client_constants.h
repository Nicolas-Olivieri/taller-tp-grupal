#ifndef CLIENT_CONSTANTS_H
#define CLIENT_CONSTANTS_H

#define TILE_SIZE 32
#define FPS 30

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 726

#define HEAD_OFFSET 21
#define GHOST_HEAD_ID 0
#define GHOST_BODY_ID 0

enum class Layer { BODY = 0, HEAD = 1, HELMET = 2, WEAPON = 3, SHIELD = 4 };

#endif  // CLIENT_CONSTANTS_H
