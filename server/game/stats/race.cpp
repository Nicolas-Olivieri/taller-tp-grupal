#include "race.h"

// TODO: PLACEHOLDERS; borrar cuando podamos leerlas desde un toml dependiendo de race_id
#define RACE_HEALTH_FACTOR 3
#define RACE_MANA_FACTOR 3
#define RACE_RECOVERY_FACTOR 3

Race::Race(uint8_t race_id):
        health_factor(RACE_HEALTH_FACTOR + /* PLACEHOLDER para uso */ race_id),
        mana_factor(RACE_MANA_FACTOR + /* PLACEHOLDER para uso */ race_id),
        recovery_factor(RACE_RECOVERY_FACTOR + /* PLACEHOLDER para uso */ race_id) {}
