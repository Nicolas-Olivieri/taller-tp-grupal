#include "player.h"


Player::Player(const std::string& player_name, const Position& position):
        player_name(player_name),
        position(position),
        direction(Direction::IDLE) {}


void Player::move(const Position& new_position,
                  const Direction& new_direction) {
    position = new_position;
    direction = new_direction;
}


Position Player::get_position() const { return position; }


Direction Player::get_direction() const { return direction; }
