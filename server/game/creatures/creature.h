#ifndef CREATURE_H
#define CREATURE_H

#include <memory>
#include <string>
#include <vector>

#include "server/command/cmd_results/interact/interact_result.h"
#include "server/game/attacker.h"
#include "server/game/killable.h"
#include "server/game/position.h"
#include "state/creaturestate.h"

#include "updatestatus.h"

enum class DropType { NOTHING = 0, GOLD = 1, USABLE = 2, EQUIPABLE = 3 };

class Creature: public Killable, public Attacker {
private:
    const uint16_t sub_id;
    std::unique_ptr<CreatureState> state;

    Player* target;

    uint8_t get_weapon_range() const;

    uint16_t get_attack_cost() const;

    bool is_in_range(const Position& other_position, uint8_t range) const;

    static uint8_t random_level(uint8_t race, uint8_t variation);

public:
    // TODO: mover métodos que no son públicos a la sección private

    Creature(const uint16_t sub_id, const uint8_t race, const uint8_t variation, const Position& position);

    std::vector<Loot> drop() override;

    CreatureUpdateStatus update_state(const Position& position, const Direction& direction);

    InteractResult interact(Player&) override;

    CreatureUpdateStatus attack_player();

    int attack() override;

    bool can_attack() const override;

    bool can_reach(const Position& other_position) const override;

    bool can_target(const Position& other_position) const;

    bool should_stop_targeting() const;

    void stop_targeting();

    void target_player(Player& player);

    bool is_targeting_someone() const;

    bool is_target_alive() const;

    void set_state(std::unique_ptr<CreatureState> state);

    bool can_reach_target();

    Position get_target_position() const;

    bool is_targeting(const Player& playter) const;

    const Stats& get_stats() const;

    const std::string& get_target_name() const;
};

#endif  // CREATURE_H
