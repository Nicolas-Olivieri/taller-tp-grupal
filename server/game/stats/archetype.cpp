#include "archetype.h"

// TODO: PLACEHOLDERS; borrar cuando podamos leerlas desde un toml dependiendo de archetype_id
#define ARCHETYPE_HEALTH_FACTOR 3
#define ARCHETYPE_MANA_FACTOR 3
#define ARCHETYPE_MEDITATION_FACTOR 3

Archetype::Archetype(uint8_t archetype_id):
        health_factor(ARCHETYPE_HEALTH_FACTOR + /* PLACEHOLDER para uso */ archetype_id),
        mana_factor(ARCHETYPE_MANA_FACTOR + /* PLACEHOLDER para uso */ archetype_id),
        meditation_factor(ARCHETYPE_MEDITATION_FACTOR + /* PLACEHOLDER para uso */ archetype_id) {}
