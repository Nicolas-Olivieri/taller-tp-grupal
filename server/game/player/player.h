#ifndef PLAYER_H
#define PLAYER_H

#include <string>

#include "../killable.h"
#include "../position.h"
#include "server/game/allies/ally.h"
#include "server/game/attacker.h"
#include "server/game/player/inventory/goldmanager.h"
#include "server/game/player/inventory/inventory.h"
#include "server/persistance/playerdata.h"
#include "server/util/calculator.h"


class Player: public Killable, public Attacker {
private:
    const std::string player_name;
    const uint8_t body;
    const uint8_t head;

    Inventory inventory;
    GoldManager gold_manager;

    Ally* bound_ally;

public:
    Player(const std::string& player_name, const PlayerData& persisted_data);

    Player(const std::string& player_name, const PlayerData& persisted_data,
           const Position& starting_position);

    /// Attacker

    // Devuelve el daño del ataque considerando equipamiento actual
    // TODO habrá que agregar a la cuenta el daño por estar cerca de compañeros del clan
    int attack() override;

    bool can_attack() const override;

    bool can_reach(const Position& other_position) const override;

    /// Killable

    void drop() override;

    InteractResult interact(Player& attacker) override;

    void update_position(const Position& new_position, const Direction& new_direction) override;

    /// Player

    const Stats& get_stats() const;

    const std::string& get_name() const;

    uint8_t get_body() const;

    uint8_t get_head() const;

    uint16_t get_safe_gold() const;

    uint16_t get_excess_gold() const;

    void earn_xp(uint32_t amount);

    void update() override;

    ~Player() override = default;

    void bind_ally(Ally* ally);

    Ally* get_bound_ally() const;

    void unbind_ally();

    void heal();
};


#endif  // PLAYER_H
