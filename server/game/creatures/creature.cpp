#include "creature.h"

#include <algorithm>
#include <cassert>
#include <utility>

#include "server/game/player/inventory/item_mapper.h"
#include "server/game/player/player.h"
#include "server/util/calculator.h"
#include "state/idlestate.h"

#define EXTRA_TARGET_RANGE 3

// TODO: todos las creatures spawnean nivel 5 de momento, después hay que hacer que puedan aparecer con
// ditintos niveles
Creature::Creature(const uint32_t sub_id, const uint8_t race, const uint8_t variation,
                   const Position& position):
        Killable(variation, race, 0, 5, position),
        sub_id(sub_id),
        state(std::make_unique<IdleState>()),
        target(nullptr) {}

void Creature::drop() { assert(false); }

InteractResult Creature::update_state(const Position& position, const Direction& direction) {
    InteractResult result = this->state->act(*this, position, direction);
    this->state->next(*this);

    return result;
}

InteractResult Creature::interact(Player& attacker) {
    target = &attacker;
    return Killable::interact(attacker);
}

InteractResult Creature::attack_player() {
    assert(can_reach(target->get_position()) && can_attack());

    if (Calculator::can_dodge(target->get_stats().agility))
        return InteractResult(AttackStatus::TARGET_DODGED);

    const uint16_t damage_applied = target->receive_damage(*this);
    const bool was_killed = !target->is_alive();

    return InteractResult(damage_applied, was_killed);
}

int Creature::attack() {
    current_attack_cooldown = required_attack_cooldown;

    const int mana_cost = ItemMapper::get_mana_cost(equipment.weapon);
    stats.mana.loose(mana_cost);

    return Calculator::calculate_damage(stats.strength, equipment);
}

bool Creature::can_attack() const {
    if (current_attack_cooldown != 0)
        return false;

    int mana_cost = ItemMapper::get_mana_cost(equipment.weapon);

    return mana_cost <= stats.mana.get_current();
}

// TODO: modularizar
bool Creature::can_reach(const Position& other_position) const {
    uint8_t range = ItemMapper::get_range(equipment.weapon);

    return std::abs(position.get_x() - other_position.get_x()) <= range and
           std::abs(position.get_y() - other_position.get_y()) <= range;
}

bool Creature::can_target(const Position& other_position) const {
    uint8_t range = ItemMapper::get_range(equipment.weapon) + EXTRA_TARGET_RANGE;

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

void Creature::set_state(std::unique_ptr<CreatureState> new_state) { state = std::move(new_state); }

Position Creature::get_target_position() const {
    if (!is_targeting_someone())
        throw std::runtime_error("The creature has no target");

    return target->get_position();
}

bool Creature::is_targeting(const Player& player) const { return target == &player; }
