#ifndef HEALTH_H
#define HEALTH_H

#include "recoverable_stat.h"


class Health: public RecoverableStat {
private:
    int constitution;

public:
    Health(int recovery_factor, int factor_class, int factor_race, int constitution, uint8_t level);

    void update_max(int level) override;

    void update() override;

    bool loose(int amount) override;
};


#endif  // HEALTH_H
