#ifndef MANA_H
#define MANA_H

#include "recoverable_stat.h"


class Mana: public RecoverableStat {
private:
    uint8_t intelligence;
    float factor_class_meditation;

public:
    Mana(uint8_t recovery_factor, float factor_class, float factor_race, float factor_class_meditation,
         uint8_t intelligence, uint8_t level);

    void update_max(uint8_t level) override;

    void meditate();

    bool loose(uint16_t amount) override;
};


#endif  // MANA_H
