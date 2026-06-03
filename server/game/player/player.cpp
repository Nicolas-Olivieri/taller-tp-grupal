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
        inventory(stats),
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
        inventory(stats),
        gold_manager(persisted_data.current_gold, persisted_data.xp_level),
        bound_ally(nullptr) {}

int Player::attack() {
    if (bound_ally != nullptr) {
        unbind_ally();
    }


    current_attack_cooldown = required_attack_cooldown;

    const int mana_cost = inventory.get_attack_cost();

    stats.mana.loose(mana_cost);

    return Calculator::calculate_damage(stats.strength, inventory.get_equipment());
}

Stats Player::get_stats() const { return stats; }

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

    int mana_cost = ItemMapper::get_mana_cost(inventory.get_equipment().weapon);

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
    const int range = inventory.get_range();
    //    std::cout << "rango: "<< range << "\n";

    return std::abs(position.get_x() - other_position.get_x()) <= range and
           std::abs(position.get_y() - other_position.get_y()) <= range;
}

InteractResult Player::interact(Player& attacker) {
    if (position == attacker.get_position()) {
        std::cout << "[Player] Jugador " << player_name << " intentó atacarse a sí mismo" << std::endl;
        return InteractResult(false);
    }

    if (not attacker.can_reach(position)) {
        std::cout << "[Player] El objetivo está demasiado lejos" << std::endl;
        return InteractResult(AttackStatus::OUT_OF_RANGE);
    }

    if (not is_alive()) {
        std::cout << "[Player] Jugador " << player_name << " fue atacado, pero ya está muerto" << std::endl;
        return InteractResult(AttackStatus::DEAD_TARGET);
    }

    if (not attacker.can_attack()) {
        std::cout << "[Player] Jugador " << player_name << " fue atacado, pero el enemigo no podía atacar"
                  << std::endl;
        return InteractResult(AttackStatus::CANNOT_ATTACK);
    }

    const int damage = attacker.attack();
    std::cout << "[Player] " << player_name << " fue atacado " << std::endl;

    if (bound_ally != nullptr) {
        unbind_ally();
    }

    if (Calculator::can_dodge(stats.agility)) {
        std::cout << "[Player] " << player_name << " ESQUIVO!" << std::endl;
        return InteractResult(AttackStatus::TARGET_DODGED);
    }

    // TODO Falta considerar
    //  - Fair game
    //  - Daño de los compis del clan?
    //  - fuego amigo (entre compis no nos pegamos)
    //  - zona segura?

    const int defense = Calculator::calculate_defense(inventory.get_equipment());

    std::cout << "[Player] daño: " << damage << "\n";
    std::cout << "[Player] defensa: " << defense << "\n";

    const int damage_applied = damage - defense;

    // TODO notificar el caso particular?
    if (damage_applied <= 0)
        return InteractResult(0, false);


    uint32_t earned_xp;
    const bool was_killed = stats.health.loose(damage_applied);

    if (was_killed) {

        earned_xp = Calculator::kill_exp(this->stats.health.get_max(), this->stats.experience.get_level(),
                                         attacker.stats.experience.get_level());
    } else {
        earned_xp = Calculator::attack_exp(damage_applied, this->stats.experience.get_level(),
                                           attacker.stats.experience.get_level());
    }

    attacker.earn_xp(earned_xp);

    return InteractResult(damage_applied, was_killed);
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
