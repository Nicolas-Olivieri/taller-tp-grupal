#include "player.h"

#include <algorithm>
#include <cassert>
#include <cstring>
#include <random>

// TODO 1: Agregar la persistencia de inventario, banco, etc... a medida que se implementen en la lógica del
// modelo

// Constructor para jugador que vuelve a conectarse
Player::Player(const std::string& player_name, const PlayerData& persisted_data):
        Killable(persisted_data.archetype, persisted_data.race, persisted_data.current_xp_amount,
                 persisted_data.xp_level, Position(persisted_data.position_x, persisted_data.position_y),
                 Equipment{persisted_data.helmet_id, persisted_data.armor_id, persisted_data.shield_id,
                           persisted_data.weapon_id}),
        player_name(player_name),
        body(persisted_data.body),
        head(persisted_data.head),
        inventory(persisted_data.inventory, persisted_data.inventory_amounts),
        bank(persisted_data.bank_gold, persisted_data.bank, persisted_data.bank_amounts),
        gold_manager(persisted_data.current_gold, persisted_data.xp_level),
        bound_ally(nullptr),
        just_resurrected(false),
        is_resurrecting(false),
        resurrection_timer(0),
        target_resurrection_position(0, 0),
        _is_founder(persisted_data.is_founder) {
    stats.health.set_current(persisted_data.current_hp);
    stats.mana.set_current(persisted_data.current_mana);

    const char* clan_ptr = reinterpret_cast<const char*>(persisted_data.clan);
    clan_name = std::string(clan_ptr, strnlen(clan_ptr, CLAN_NAME));
}

// Constructor para jugador que se conecta por primera vez
Player::Player(const std::string& player_name, const PlayerData& persisted_data,
               const Position& starting_position):
        Killable(persisted_data.archetype, persisted_data.race, persisted_data.current_xp_amount,
                 persisted_data.xp_level, starting_position,
                 Equipment{persisted_data.helmet_id, persisted_data.armor_id, persisted_data.shield_id,
                           persisted_data.weapon_id}),
        player_name(player_name),
        body(persisted_data.body),
        head(persisted_data.head),
        inventory(),
        bank(persisted_data.bank_gold, persisted_data.bank, persisted_data.bank_amounts),
        gold_manager(persisted_data.current_gold, persisted_data.xp_level),
        bound_ally(nullptr),
        just_resurrected(false),
        is_resurrecting(false),
        resurrection_timer(0),
        target_resurrection_position(0, 0),
        _is_founder(false),
        clan_name("") {}

int Player::attack() {
    if (bound_ally != nullptr) {
        unbind_ally();
    }

    current_attack_cooldown = required_attack_cooldown;

    WeaponData data = GameConfig::get().get_weapon(equipment.weapon);
    stats.mana.loose(data.mana_cost);

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

    WeaponData data = GameConfig::get().get_weapon(equipment.weapon);

    std::cout << "[Player] costo de mana: " << data.mana_cost << "\n";
    std::cout << "[Player] mana actual: " << stats.mana.get_current() << "\n";

    return data.mana_cost <= stats.mana.get_current();
}

void Player::update() {
    Killable::update();

    if (is_resurrecting) {
        resurrection_timer--;
        if (resurrection_timer <= 0) {
            complete_delayed_resurrection();
        }
    }

    stats.health.update();

    if (is_meditating) {
        stats.mana.meditate();
    } else {
        stats.mana.update();
    }
}

bool Player::can_reach(const Position& other_position) const {
    WeaponData data = GameConfig::get().get_weapon(equipment.weapon);
    uint8_t range = data.range;
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
    //      (en update del GameWorld -> por cada jugador recorremos las n casillas más cercanas y desde
    //      ahí player expone un método que le agrega boost y lo usa en su attack)
    //  - fuego amigo (entre compis no nos pegamos)
    //  - zona segura?

    InteractResult result = Killable::interact(attacker);
    result.attack.player_attacked = player_name;

    return result;
}

void Player::update_position(const Position& new_position, const Direction& new_direction) {
    if (bound_ally != nullptr) {
        unbind_ally();
    }
    Killable::update_position(new_position, new_direction);
}

std::vector<Loot> Player::drop() {
    std::vector<Loot> drops;

    drop_excess_gold(drops);
    drop_inventory(drops);
    drop_equipment(drops);

    static std::random_device rd;
    static std::mt19937 generator(rd());

    // Randomizamos el orden del Loot para que no sepas que te va a tocar!
    std::shuffle(drops.begin(), drops.end(), generator);

    return drops;
}

void Player::drop_excess_gold(std::vector<Loot>& drops) {
    uint16_t excess_gold = gold_manager.get_excess_gold();
    if (excess_gold > 0) {
        gold_manager.spend(excess_gold);
        drops.push_back(Loot(excess_gold));
    }
}

void Player::drop_inventory(std::vector<Loot>& drops) {
    for (const auto& [item, amount]: inventory.get_items()) {
        for (size_t i = 0; i < amount; i++) {
            if (item != NO_ITEM)
                drops.push_back(Loot(item));
        }
    }

    inventory.clear();
}

void Player::drop_equipment(std::vector<Loot>& drops) {
    if (equipment.weapon != NO_ITEM)
        drops.push_back(Loot(equipment.weapon));
    if (equipment.armor != NO_ITEM)
        drops.push_back(Loot(equipment.armor));
    if (equipment.shield != NO_ITEM)
        drops.push_back(Loot(equipment.shield));
    if (equipment.helmet != NO_ITEM)
        drops.push_back(Loot(equipment.helmet));

    equipment.weapon = NO_ITEM;
    equipment.armor = NO_ITEM;
    equipment.shield = NO_ITEM;
    equipment.helmet = NO_ITEM;
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

void Player::use_item(const uint8_t item_id) { inventory.use_item(stats, equipment, item_id); }

void Player::unequip_item(const uint8_t item_id) { inventory.unequip_item(equipment, item_id); }

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

const Equipment& Player::get_equipment() const { return this->equipment; }

const std::unordered_map<uint8_t, uint8_t>& Player::get_inventory_items() const {
    return inventory.get_items();
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

bool Player::can_move() const { return not is_resurrecting and Killable::can_move(); }

void Player::start_delayed_resurrection(const int wait_time, const Position& position) {
    is_resurrecting = true;
    resurrection_timer = wait_time;
    target_resurrection_position = Position(position.get_x() + 1, position.get_y());
}

bool Player::did_just_resurrect() {
    if (just_resurrected) {
        just_resurrected = false;
        return true;
    }

    return false;
}

void Player::complete_delayed_resurrection() {
    position = target_resurrection_position;
    is_resurrecting = false;
    just_resurrected = true;
    heal();
    std::cout << "[Player] " << player_name << " ha resucitado junto al sacerdote." << std::endl;
}

std::string Player::get_clan_name() const { return clan_name; }

void Player::join_clan(const std::string& _clan_name) {
    clan_name = _clan_name;
    _is_founder = false;
}

void Player::found_clan(const std::string& _clan_name) {
    clan_name = _clan_name;
    _is_founder = true;
}

void Player::leave_clan() {
    clan_name.clear();
    _is_founder = false;
}

bool Player::is_clan_founder() const { return _is_founder; }

void Player::set_xp_level(const uint8_t new_level) {
    stats.experience.set_level(new_level);
    stats.upgrade();
}
