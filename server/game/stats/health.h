#ifndef HEALTH_H
#define HEALTH_H

#include "recoverable_stat.h"


class Health: public RecoverableStat {
private:
    uint8_t& constitution;

public:
    Health(uint8_t recovery_factor, float factor_class, float factor_race, uint8_t& constitution,
           uint8_t level);

    void update_max(uint8_t level) override;

    void update() override;

    bool loose(uint16_t amount) override;
};


#endif  // HEALTH_H
