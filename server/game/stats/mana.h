#ifndef MANA_H
#define MANA_H

#include "recoverable_stat.h"


class Mana: public RecoverableStat {
private:
    int intelligence;
    int factor_class_meditation;

public:
    Mana(int recovery_factor, int factor_class, int factor_race, int factor_class_meditation,
         int intelligence, uint8_t level);

    void update_max(int level) override;

    void meditate();

    bool loose(int amount) override;
};


#endif  // MANA_H
