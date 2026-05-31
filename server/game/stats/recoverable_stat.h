#ifndef RECOVERABLE_STAT_H
#define RECOVERABLE_STAT_H

#include <cstdint>

class RecoverableStat {
protected:
    const int recovery_factor;
    const int factor_class;
    const int factor_race;

    int max_amount;
    int current_amount;
    int tick_accumulator;

    explicit RecoverableStat(int recovery_factor, int factor_class, int factor_race, int max);

public:
    virtual void update_max(int level) = 0;

    virtual void update();

    virtual bool loose(int amount) = 0;

    int get_current() const;

    virtual ~RecoverableStat() = default;
};


#endif  // RECOVERABLE_STAT_H
