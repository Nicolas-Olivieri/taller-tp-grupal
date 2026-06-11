#include "creature.h"

#include <algorithm>
#include <cassert>
#include <utility>

#include "server/config/game_config.h"
#include "server/game/player/player.h"
#include "server/util/calculator.h"
#include "state/idlestate.h"

#define EXTRA_TARGET_RANGE 5  // TODO: toml

// TODO: todos las creatures spawnean nivel 5 de momento, después hay que hacer que puedan aparecer con
// ditintos niveles
Creature::Creature(const uint16_t sub_id, const uint8_t race, const uint8_t variation,
                   const Position& position):
        Killable(race, variation, random_level(race, variation), position, equip_items(variation)),
        sub_id(sub_id),
        state(&IdleState::get()),
        target(nullptr) {}

uint8_t Creature::random_level(uint8_t race, uint8_t variation) {
    GameConfig& config = GameConfig::get();

    uint8_t level = Calculator::calculate_creature_level(
            config.get_creature_base_level(race), config.get_variation(variation).max_level_multiplier);

    std::cout << "Creature level: " << static_cast<int>(level) << std::endl;

    return level;
}

Equipment Creature::equip_items(uint8_t variation) {
    GameConfig& config = GameConfig::get();
    Equipment equipment{NO_ITEM, NO_ITEM, NO_ITEM, NO_ITEM};

    const std::vector<uint8_t>& items = config.get_variation(variation).equipment;

    for (const auto& item: items) {
        if (config.weapons_contains(item))
            equipment.weapon = item;
        else if (config.armors_contains(item))
            equipment.armor = item;
        else if (config.helmets_contains(item))
            equipment.helmet = item;
        else if (config.shields_contains(item))
            equipment.shield = item;
    }

    return equipment;
}

std::vector<Loot> Creature::drop() {
    std::vector<Loot> drop;

    const DropProbabilitiesData& data = GameConfig::get().get_drop_probabilities();

    std::vector<float> probabilities = {data.nothing, data.gold, data.usable, data.equipable};
    int index = Calculator::random_from_weighted_probabilities(probabilities);

    GameConfig& config = GameConfig::get();

    switch (static_cast<DropType>(index)) {
        case DropType::NOTHING:
            break;
        case DropType::GOLD:
            drop.push_back(Loot(Calculator::calculate_random_drop_gold(stats.health.get_max())));
            break;
        case DropType::USABLE:
            drop.push_back(
                    Loot(Calculator::random_number(config.get_min_usable_id(), config.get_max_usable_id())));
            break;
        case DropType::EQUIPABLE: {
            uint8_t item =
                    Calculator::random_number(config.get_min_equipable_id(), config.get_max_equipable_id());
            // TODO: NI BIEN SE IMPLEMENTE EL BÁCULO DE CURACIÓN SACAR ESTE HARDCODEO
            if (item == 0 or item == 5)  // TODO: CORREGIME
                item++;                  // TODO: CORREGIME
            drop.push_back(Loot(item));  // TODO: CORREGIME
            // TODO: CORREGIRRRR
        } break;
        default:
            throw std::invalid_argument("There is no known way to drop something of this type");
    }

    return drop;
}

void Creature::update_state() { this->state = this->state->next(*this); }

InteractResult Creature::interact(Player& attacker) {
    target = &attacker;
    return Killable::interact(attacker);
}

CreatureUpdateStatus Creature::attack_player() {
    assert(can_reach(target->get_position()) && can_attack());

    const uint16_t damage = attack();

    if (Calculator::can_dodge(target->get_stats().agility)) {
        return CreatureUpdateStatus(stats.race_id, target->get_name(), 0, false);
    }

    const uint16_t damage_applied = target->receive_damage(damage);
    const bool was_killed = !target->is_alive();

    return CreatureUpdateStatus(stats.race_id, target->get_name(), damage_applied, was_killed);
}

int Creature::attack() {
    current_attack_cooldown = required_attack_cooldown;

    const int mana_cost = get_attack_cost();
    stats.mana.loose(mana_cost);

    return Calculator::calculate_damage(stats.strength, equipment);
}

bool Creature::can_attack() const {
    if (current_attack_cooldown != 0)
        return false;

    int mana_cost = get_attack_cost();

    return mana_cost <= stats.mana.get_current();
}

// TODO: modularizar
bool Creature::can_reach(const Position& other_position) const {
    uint8_t range = get_weapon_range();
    return is_in_range(other_position, range);
}

bool Creature::can_target(const Position& other_position) const {
    uint8_t range = get_weapon_range() + EXTRA_TARGET_RANGE;
    return is_in_range(other_position, range);
}

uint8_t Creature::get_weapon_range() const {
    WeaponData data = GameConfig::get().get_weapon(equipment.weapon);
    return data.range;
}

uint16_t Creature::get_attack_cost() const {
    WeaponData data = GameConfig::get().get_weapon(equipment.weapon);
    return data.mana_cost;
}

bool Creature::is_in_range(const Position& other_position, uint8_t range) const {
    return std::abs(position.get_x() - other_position.get_x()) <= range and
           std::abs(position.get_y() - other_position.get_y()) <= range;
}

bool Creature::should_stop_targeting() const {
    return !is_target_alive() || !can_target(target->get_position());
}

void Creature::stop_targeting() { target = nullptr; }

void Creature::target_player(Player& player) { target = &player; }

bool Creature::is_targeting_someone() const {
    if (target != nullptr && !target->is_alive())
        return false;

    return target != nullptr;
}

bool Creature::is_target_alive() const { return is_targeting_someone() && target->is_alive(); }

bool Creature::can_reach_target() { return is_targeting_someone() && can_reach(target->get_position()); }

Position Creature::get_target_position() const {
    if (!is_targeting_someone())
        throw std::runtime_error("The creature has no target");

    return target->get_position();
}

bool Creature::is_targeting(const Player& player) const { return target == &player; }

const Stats& Creature::get_stats() const { return stats; }

const std::string& Creature::get_target_name() const { return target->get_name(); }
