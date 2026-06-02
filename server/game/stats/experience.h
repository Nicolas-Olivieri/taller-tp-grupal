#ifndef EXPERIENCE_H
#define EXPERIENCE_H

#include <cstdint>


class Experience {
private:
    uint32_t current_amount;
    uint8_t level;

public:
    Experience(uint32_t current_amount, uint8_t level);

    uint8_t get_level() const;

    uint32_t get_current_amount() const;

    bool earn_xp(uint32_t amount);
};


#endif  // EXPERIENCE_H
