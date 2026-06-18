#ifndef EDITOR_CONSTANTS_H
#define EDITOR_CONSTANTS_H

enum class EditorMode { DRAW, ERASE, DRAG, SAFE_ZONE };

enum class ImageType { TILE, COLLIDER, NPC };

#define WALKABLE_TILE 1
#define UNWALKABLE_TILE 0

#endif  // EDITOR_CONSTANTS_H
