#include "player.h"

#define ATTACK_COOLDOWN 30
#define MOVE_COOLDOWN 6


Player::Player(const std::string& player_name, const Position& position):
        Killable(ATTACK_COOLDOWN, MOVE_COOLDOWN),
        player_name(player_name),
        position(position),
        direction(Direction::IDLE) {}


void Player::move(const Position& new_position, const Direction& new_direction) {
    position = new_position;
    direction = new_direction;
    move_cooldown = MOVE_COOLDOWN;
}


Position Player::get_position() const { return position; }


Direction Player::get_direction() const { return direction; }


bool Player::can_attack() const { return attack_cooldown == 0; }


bool Player::can_move() const { return move_cooldown == 0; }


void Player::update(const int iteration) {
    move_cooldown -= iteration;
    if (move_cooldown < 0) {
        move_cooldown = 0;
    }
}
