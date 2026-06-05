#include "player.h"

#include <cassert>

#include "server/game/player/inventory/item_mapper.h"


// TODO 1: Agregar la persistencia de inventario, banco, etc... a medida que se implementen en la lógica del
// modelo

// Constructor para jugador que vuelve a conectarse
Player::Player(const std::string& player_name, const PlayerData& persisted_data):
        Killable(persisted_data.archetype, persisted_data.race, persisted_data.current_xp_amount,
                 persisted_data.xp_level, Position(persisted_data.position_x, persisted_data.position_y)),
        player_name(player_name),
        body(persisted_data.body),
        head(persisted_data.head),
        inventory(equipment),
        bank(persisted_data.bank_gold, persisted_data.bank, persisted_data.bank_amounts),
        gold_manager(persisted_data.current_gold, persisted_data.xp_level),
        bound_ally(nullptr) {
    stats.health.set_current(persisted_data.current_hp);
    stats.mana.set_current(persisted_data.current_mana);
}

// Constructor para jugador que se conecta por primera vez
Player::Player(const std::string& player_name, const PlayerData& persisted_data,
               const Position& starting_position):
        Killable(persisted_data.archetype, persisted_data.race, persisted_data.current_xp_amount,
                 persisted_data.xp_level, starting_position),
        player_name(player_name),
        body(persisted_data.body),
        head(persisted_data.head),
        inventory(equipment),
        bank(persisted_data.bank_gold, persisted_data.bank, persisted_data.bank_amounts),
        gold_manager(persisted_data.current_gold, persisted_data.xp_level),
        bound_ally(nullptr) {}

int Player::attack() {
    if (bound_ally != nullptr) {
        unbind_ally();
    }

    current_attack_cooldown = required_attack_cooldown;

    const int mana_cost = inventory.get_attack_cost(equipment);

    stats.mana.loose(mana_cost);

    return Calculator::calculate_damage(stats.strength, equipment);
}

const Stats& Player::get_stats() const { return stats; }

const std::string& Player::get_name() const { return player_name; }

uint8_t Player::get_body() const { return body; }

uint8_t Player::get_head() const { return head; }

uint16_t Player::get_safe_gold() const { return gold_manager.get_safe_gold(); }

uint16_t Player::get_excess_gold() const { return gold_manager.get_excess_gold(); }

void Player::earn_xp(uint32_t amount) {
    if (stats.experience.earn_xp(amount))
        stats.upgrade();
}

bool Player::can_attack() const {
    if (current_attack_cooldown != 0)
        return false;

    if (not is_alive()) {
        std::cout << "[Player] Jugador " << player_name << " intentó atacar, pero está muerto" << std::endl;
        return false;
    }

    int mana_cost = ItemMapper::get_mana_cost(equipment.weapon);

    std::cout << "[Player] costo de mana: " << mana_cost << "\n";
    std::cout << "[Player] mana actual: " << stats.mana.get_current() << "\n";

    return mana_cost <= stats.mana.get_current();
}

void Player::update() {
    Killable::update();

    stats.health.update();

    if (is_meditating) {
        stats.mana.meditate();
    } else {
        stats.mana.update();
    }
}

bool Player::can_reach(const Position& other_position) const {
    uint8_t range = inventory.get_range(equipment);
    //    std::cout << "rango: "<< range << "\n";

    return std::abs(position.get_x() - other_position.get_x()) <= range and
           std::abs(position.get_y() - other_position.get_y()) <= range;
}

InteractResult Player::interact(Player& attacker) {
    if (position == attacker.get_position())
        return InteractResult();

    if (not is_alive())
        return InteractResult(AttackStatus::DEAD_TARGET);

    // TODO Falta considerar
    //  - Fair game
    //  - Daño de los compis del clan?
    //      (en update del GameWorld -> por cada jugador recorremos las n casillas más cercanas y desde ahí
    //      player expone un método que le agrega boost y lo usa en su attack)
    //  - fuego amigo (entre compis no nos pegamos)
    //  - zona segura?

    return Killable::interact(attacker);
}

void Player::update_position(const Position& new_position, const Direction& new_direction) {
    if (bound_ally != nullptr) {
        unbind_ally();
    }
    Killable::update_position(new_position, new_direction);
}

void Player::drop() {
    uint16_t excess_gold = gold_manager.get_excess_gold();
    gold_manager.spend(excess_gold);

    // TODO: hacer que se dropeen

    assert(false);
}

void Player::bind_ally(Ally* ally) {
    std::cout << "[Player] El jugador " << player_name << " se vinculó con un aliado" << std::endl;
    bound_ally = ally;
}

Ally* Player::get_bound_ally() const { return bound_ally; }

void Player::unbind_ally() {
    std::cout << "[Player] El jugador " << player_name << " se desvinculó del aliado" << std::endl;
    bound_ally = nullptr;
}

void Player::heal() {
    stats.health.recover_all();
    stats.mana.recover_all();
}

void Player::spend_gold(const uint16_t amount) { gold_manager.spend(amount); }

void Player::add_gold(const uint16_t amount) { gold_manager.add(amount); }

void Player::acquire_item(const uint8_t item_id) { inventory.acquire_item(item_id); }

void Player::drop_item(const uint8_t item_id) { inventory.drop_item(equipment, item_id); }

uint16_t Player::get_bank_gold() const { return bank.get_gold(); }

void Player::deposit_gold_to_bank(const uint16_t amount) {
    gold_manager.spend(amount);
    try {
        bank.deposit_gold(amount);

    } catch (const BankFull&) {
        gold_manager.add(amount);
        throw;
    }
}

void Player::withdraw_gold_from_bank(const uint16_t amount) {
    bank.withdraw_gold(amount);
    gold_manager.add(amount);
}

const std::map<uint8_t, uint8_t>& Player::get_bank_items() const { return bank.get_items(); }

void Player::deposit_item_to_bank(const uint8_t item_id) {
    inventory.drop_item(equipment, item_id);
    try {
        bank.deposit_item(item_id);

    } catch (const BankFull&) {
        inventory.acquire_item(item_id);
        throw;
    }
}

void Player::withdraw_item_from_bank(const uint8_t item_id) {
    bank.withdraw_item(item_id);
    try {
        inventory.acquire_item(item_id);

    } catch (const InventoryFull&) {
        bank.deposit_item(item_id);
        throw;
    }
}
