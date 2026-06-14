#ifndef EDITOR_CONSTANTS_H
#define EDITOR_CONSTANTS_H

enum class EditorMode { DRAW, ERASE, DRAG, SAFE_ZONE };

enum class ImageType { TILE, COLLIDER, NPC };

#define HEADER 0xFAF4
#define SAFE_ZONE_ID 0
#define WALKABLE_TILE 1
#define UNWALKABLE_TILE 0

#define TILE_SIZE 32
#define SAFE_ZONE_BRUSH_W 4
#define SAFE_ZONE_BRUSH_H 4


#endif  // EDITOR_CONSTANTS_H
