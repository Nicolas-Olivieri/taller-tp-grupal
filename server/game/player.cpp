#include "player.h"


Player::Player(const std::string& player_name, const Position& position):
        player_name(player_name), position(position) {}


void Player::move(const Position& new_position) { position = new_position; }


Position Player::get_position() const { return position; }
