#include "heal_command.h"

#include <map>


HealCommand::HealCommand(const std::string& player_name): player_name(player_name), result() {}


void HealCommand::execute(GameWorld& world) { result = world.heal_player(player_name); }


void HealCommand::build_snapshot(SnapshotBuilder& builder) {
    if (result.status == HealStatus::PLAYER_UNBOUNDED) {
        const std::string& error_message = "Hablale a un sacerdote para pedirle que te cure";
        builder.add_action(ActionDTO(ChatMessageDTO(MessageType::ERROR, player_name, error_message)));
        return;
    }

    static std::map<AllyType, std::string> ally_type_to_string({
            {AllyType::PRIEST, "Sacerdote"},
            {AllyType::MERCHANT, "Comerciante"},
            {AllyType::BANKER, "Banquero"},
    });

    if (not ally_type_to_string.contains(result.ally)) {
        throw std::runtime_error("HealCommand recibió un NPC aliado desconocido");
    }

    static std::map<HealStatus, std::string> result_to_message(
            {{HealStatus::PLAYER_HEALED, "A sus ordenes! Toda tu vida y mana fueron recargadas"},
             {HealStatus::PLAYER_IS_DEAD, "Estas muerto! Primero tenes que resucitar"},
             {HealStatus::ACTION_NOT_ACCEPTED, "Perdon, yo no puedo hacer eso"}});

    if (not result_to_message.contains(result.status)) {
        throw std::runtime_error("HealCommand recibió un resultado incorrecto");
    }

    const std::string& sender = ally_type_to_string.at(result.ally);
    const std::string& content = result_to_message.at(result.status);
    builder.add_action(ActionDTO(ChatMessageDTO(MessageType::ALLY, sender, player_name, content)));

    if (result.status == HealStatus::PLAYER_HEALED)
        builder.add_action(ActionDTO(HealDTO(player_name)));
}
