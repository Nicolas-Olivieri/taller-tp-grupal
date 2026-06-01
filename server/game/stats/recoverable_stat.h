#ifndef RECOVERABLE_STAT_H
#define RECOVERABLE_STAT_H

#include <cstdint>

class RecoverableStat {
protected:
    const uint8_t recovery_factor;
    const float factor_class;
    const float factor_race;

    uint16_t max_amount;
    uint16_t current_amount;
    int tick_accumulator;

    RecoverableStat(uint8_t recovery_factor, float factor_class, float factor_race, uint8_t max);

public:
    virtual void update_max(uint8_t level) = 0;

    virtual void update();

    virtual bool loose(uint16_t amount) = 0;

    uint16_t get_max() const;

    uint16_t get_current() const;

    void set_current(int amount);

    virtual ~RecoverableStat() = default;
};


#endif  // RECOVERABLE_STAT_H
