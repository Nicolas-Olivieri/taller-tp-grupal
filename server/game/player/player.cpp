#include "player.h"

// TODO: Todas estas constantes deberían salir del TOML

#define ATTACK_COOLDOWN 30
#define MOVE_COOLDOWN 6


// TODO 1: Agregar la persistencia de inventario, banco, etc... a medida que se implementen en la lógica del
// modelo
// TODO 2: no olvidar inicializar la vida con lo que le quedaba cuando se desconectó, las RecoverableStat
// deberían exponer algo para ello
Player::Player(const std::string& player_name, const PlayerData& persisted_data):
        Killable(ATTACK_COOLDOWN, MOVE_COOLDOWN, persisted_data.archetype, persisted_data.race,
                 persisted_data.current_xp_amount, persisted_data.xp_level),
        player_name(player_name),
        body(persisted_data.body),
        head(persisted_data.head),
        position(persisted_data.position_x, persisted_data.position_y),
        direction(Direction::IDLE) {}

Player::Player(const std::string& player_name, const PlayerData& persisted_data,
               const Position& starting_position):
        Killable(ATTACK_COOLDOWN, MOVE_COOLDOWN, persisted_data.archetype, persisted_data.race,
                 persisted_data.current_xp_amount, persisted_data.xp_level),
        player_name(player_name),
        body(persisted_data.body),
        head(persisted_data.head),
        position(starting_position),
        direction(Direction::IDLE) {}

void Player::update_position(const Position& new_position, const Direction& new_direction) {
    position = new_position;
    direction = new_direction;
    move_cooldown = MOVE_COOLDOWN;
}


void Player::attack() { attack_cooldown = ATTACK_COOLDOWN; }

Stats Player::get_stats() const { return stats; }

Position Player::get_position() const { return position; }


Direction Player::get_direction() const { return direction; }

uint8_t Player::get_body() const { return body; }

uint8_t Player::get_head() const { return head; }

bool Player::can_attack() const { return attack_cooldown == 0; }


bool Player::can_move() const { return move_cooldown == 0; }

void Player::earn_xp(uint32_t amount) {
    if (stats.experience.earn_xp(amount))
        stats.upgrade();
}

void Player::update() {
    attack_cooldown--;
    if (attack_cooldown < 0) {
        attack_cooldown = 0;
    }

    move_cooldown--;
    if (move_cooldown < 0) {
        move_cooldown = 0;
    }

    stats.health.update();

    if (is_meditating) {
        stats.mana.meditate();
    } else {
        stats.mana.update();
    }
}
