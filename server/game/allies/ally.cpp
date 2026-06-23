#include "ally.h"

#include <map>

#include "server/game/player/player.h"


Ally::Ally(const Position& position, const AllyType& type): position(position), type(type) {}


InteractResult Ally::interact(Player& player) {
    player.bind_ally(this);

    return InteractResult(type);
}


Position Ally::get_position() const { return position; }


AllyType Ally::get_type() const { return type; }

std::string Ally::ally_type_to_string(const AllyType& type, const std::string& caller) {
    static std::map<AllyType, std::string> ally_type_to_string({
            {AllyType::PRIEST, "Sacerdote"},
            {AllyType::MERCHANT, "Comerciante"},
            {AllyType::BANKER, "Banquero"},
            {AllyType::TOTEM, "Totem"},
    });

    if (not ally_type_to_string.contains(type))
        throw std::runtime_error(caller + " encontró un NPC aliado desconocido");

    return ally_type_to_string.at(type);
}
