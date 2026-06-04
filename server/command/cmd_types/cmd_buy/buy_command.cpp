#include "buy_command.h"

#include <map>

BuyCommand::BuyCommand(const std::string& player_name, const uint8_t item_id):
        player_name(player_name), item_id(item_id), result() {}


void BuyCommand::execute(GameWorld& world) { result = world.buy_item(player_name, item_id); }


void BuyCommand::build_snapshot(SnapshotBuilder& builder) {
    if (result.status == BuyStatus::PLAYER_UNBOUNDED) {
        const std::string& error_message = "Hablale a un sacerdote o comerciante para poder comprar un item";
        builder.add_action(ActionDTO(ChatMessageDTO(MessageType::ERROR, player_name, error_message)));
        return;
    }

    static std::map<AllyType, std::string> ally_type_to_string({
            {AllyType::PRIEST, "Sacerdote"},
            {AllyType::MERCHANT, "Comerciante"},
    });

    if (not ally_type_to_string.contains(result.ally)) {
        throw std::runtime_error("BuyCommand recibió un NPC aliado desconocido");
    }

    static std::map<BuyStatus, std::string> result_to_message(
            {{BuyStatus::ITEM_SOLD, "Gracias por tu compra"},
             {BuyStatus::ITEM_NOT_SOLD, "Yo no vendo ese item"},
             {BuyStatus::NOT_ENOUGH_GOLD, "No te alcanza el oro para comprar ese item"},
             {BuyStatus::ACTION_NOT_ACCEPTED, "Perdon, yo no puedo hacer eso"}});

    if (not result_to_message.contains(result.status)) {
        throw std::runtime_error("BuyCommand recibió un resultado incorrecto");
    }

    const std::string& sender = ally_type_to_string.at(result.ally);
    const std::string& content = result_to_message.at(result.status);
    builder.add_action(ActionDTO(ChatMessageDTO(MessageType::ALLY, sender, player_name, content)));
}
