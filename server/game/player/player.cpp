#include "player.h"

// TODO: Todas estas constantes deberían salir del TOML

#define ATTACK_COOLDOWN 30
#define MOVE_COOLDOWN 6

#define AGILITY 10
#define INTELLIGENCE 10
#define CONSTITUTION 10
#define STRENGTH 10

#define ARCHETYPE_HEALTH_FACTOR 3
#define ARCHETYPE_MANA_FACTOR 3
#define ARCHETYPE_MEDITATION_FACTOR 3

#define RACE_HEALTH_FACTOR 3
#define RACE_MANA_FACTOR 3
#define RACE_RECOVERY_FACTOR 3


Player::Player(const std::string& player_name, const Position& position):
        Killable(ATTACK_COOLDOWN, MOVE_COOLDOWN, AGILITY, CONSTITUTION, INTELLIGENCE, STRENGTH,
                 Archetype(ARCHETYPE_HEALTH_FACTOR, ARCHETYPE_MANA_FACTOR, ARCHETYPE_MEDITATION_FACTOR),
                 Race(RACE_HEALTH_FACTOR, RACE_MANA_FACTOR, RACE_RECOVERY_FACTOR)),
        player_name(player_name),
        archetype(ARCHETYPE_HEALTH_FACTOR, ARCHETYPE_MANA_FACTOR, ARCHETYPE_MEDITATION_FACTOR),
        race(RACE_HEALTH_FACTOR, RACE_MANA_FACTOR, RACE_RECOVERY_FACTOR),
        position(position),
        direction(Direction::IDLE) {}


void Player::update_position(const Position& new_position, const Direction& new_direction) {
    position = new_position;
    direction = new_direction;
    move_cooldown = MOVE_COOLDOWN;
}


void Player::attack() { attack_cooldown = ATTACK_COOLDOWN; }


Position Player::get_position() const { return position; }


Direction Player::get_direction() const { return direction; }


bool Player::can_attack() const { return attack_cooldown == 0; }


bool Player::can_move() const { return move_cooldown == 0; }


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
