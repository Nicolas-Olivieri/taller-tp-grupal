#ifndef EXPERIENCE_H
#define EXPERIENCE_H

#include <cstdint>


class Experience {
private:
    uint32_t current_amount;
    uint8_t level;

public:
    Experience();

    uint8_t get_level() const;

    uint32_t get_current_amount() const;

    void earn_xp(uint32_t amount);
};


#endif  // EXPERIENCE_H
