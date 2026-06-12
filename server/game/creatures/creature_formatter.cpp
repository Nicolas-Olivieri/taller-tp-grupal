#include "creature_formatter.h"

#include <format>

// TODO estas constantes están raras aca, capaz habría que llevarlas a un toml?
std::string CreatureFormatter::get_creature_name(uint8_t creature_id) {
    static std::unordered_map<uint8_t, std::string> creature_to_name = {
            {0, "Goblin"}, {1, "Esqueleto"}, {2, "Zombie"}, {3, "Araña"}, {4, "Orco"}, {5, "Golem"}};

    return creature_to_name.at(creature_id);
}

std::string CreatureFormatter::get_attack_message(const CreatureUpdateStatus& status) {
    const std::string creature_name = get_creature_name(status.creature_id);

    if (status.damage_dealt == 0) {
        return std::format("Esquivaste el ataque de {}!!", creature_name);
    } else if (status.killed_target) {
        return std::format("{} te mató", creature_name);
    }

    return std::format("{} te quitó {} de vida", creature_name, status.damage_dealt);
}

std::string CreatureFormatter::get_clan_attack_message(const CreatureUpdateStatus& status) {
    const std::string creature_name = get_creature_name(status.creature_id);

    if (status.killed_target)
        return std::format("{} mató a {}", creature_name, status.player_name);


    return std::format("{} le quito {} de vida a {}", creature_name, status.damage_dealt, status.player_name);
}
