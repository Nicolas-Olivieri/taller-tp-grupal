#include "player.h"


// TODO 1: Agregar la persistencia de inventario, banco, etc... a medida que se implementen en la lógica del
// modelo

// Constructor para jugador que vuelve a conectarse
Player::Player(const std::string& player_name, const PlayerData& persisted_data):
        Killable(persisted_data.archetype, persisted_data.race, persisted_data.current_xp_amount,
                 persisted_data.xp_level, Position(persisted_data.position_x, persisted_data.position_y)),
        player_name(player_name),
        body(persisted_data.body),
        head(persisted_data.head) {
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
        head(persisted_data.head) {}


Stats Player::get_stats() const { return stats; }

uint8_t Player::get_body() const { return body; }

uint8_t Player::get_head() const { return head; }

void Player::earn_xp(uint32_t amount) {
    if (stats.experience.earn_xp(amount))
        stats.upgrade();
}

void Player::update() {
    current_attack_cooldown--;
    if (current_attack_cooldown <= 0) {
        current_attack_cooldown = 0;
    }

    current_move_cooldown--;
    if (current_move_cooldown <= 0) {
        current_move_cooldown = 0;
    }

    stats.health.update();

    if (is_meditating) {
        stats.mana.meditate();
    } else {
        stats.mana.update();
    }
}
