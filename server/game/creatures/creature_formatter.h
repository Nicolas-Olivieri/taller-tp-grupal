#ifndef CREATURE_FORMATTER_H
#define CREATURE_FORMATTER_H


#include <cstdint>
#include <string>
#include <unordered_map>

#include "updatestatus.h"

class CreatureFormatter {
public:
    static std::string get_attack_message(const CreatureUpdate& update);

    static std::string get_clan_attack_message(const CreatureUpdate& update);

private:
    static std::string get_creature_name(uint8_t creature_id);
};


#endif  // CREATURE_FORMATTER_H
