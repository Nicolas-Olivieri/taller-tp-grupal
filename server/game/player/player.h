#ifndef PLAYER_H
#define PLAYER_H

#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include "../killable.h"
#include "../position.h"
#include "bank/bank.h"
#include "server/game/allies/ally.h"
#include "server/game/attacker.h"
#include "server/game/player/inventory/goldmanager.h"
#include "server/game/player/inventory/inventory.h"
#include "server/persistance/playerdata.h"
#include "server/util/calculator.h"

#include "clan_membership.h"


class Player: public Killable, public Attacker {
private:
    const std::string player_name;
    const uint8_t body;
    const uint8_t head;

    Inventory inventory;
    Bank bank;
    GoldManager gold_manager;

    Ally* bound_ally;

    // TODO: Considerar usar un enum para los estados (vivo, resucitando, muerto)
    bool just_resurrected;
    bool is_resurrecting;
    int resurrection_timer;
    Position target_resurrection_position;

    void drop_excess_gold(std::vector<Loot>& drops);

    void drop_inventory(std::vector<Loot>& drops);

    void drop_equipment(std::vector<Loot>& drops);

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

    std::vector<Loot> drop() override;

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

    void spend_gold(uint16_t amount);

    void add_gold(uint16_t amount);

    void use_item(uint8_t item_id);

    void unequip_item(const uint8_t item_id);

    void acquire_item(uint8_t item_id);

    void drop_item(uint8_t item_id);

    uint16_t get_bank_gold() const;

    void deposit_gold_to_bank(uint16_t amount);

    void withdraw_gold_from_bank(uint16_t amount);

    const Equipment& get_equipment() const;

    const std::unordered_map<uint8_t, uint8_t>& get_inventory_items() const;

    const std::map<uint8_t, uint8_t>& get_bank_items() const;

    void deposit_item_to_bank(uint8_t item_id);

    void withdraw_item_from_bank(uint8_t item_id);

    bool can_move() const override;

    void start_delayed_resurrection(int wait_time, const Position& position);

    bool did_just_resurrect();

    std::string get_clan_name() const;

    void join_clan(const std::string& clan_name);

    void found_clan(const std::string& clan_name);

    void leave_clan();

    bool is_clan_founder() const;

    void set_xp_level(const uint8_t new_level);

    void set_near_clan_mates(const uint8_t near_clan_mates_amount);

    void meditate();

private:
    void complete_delayed_resurrection();
};


#endif  // PLAYER_H
