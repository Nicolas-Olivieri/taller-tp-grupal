#include "buy_command.h"

#include <map>

BuyCommand::BuyCommand(const std::string& player_name, const uint8_t item_id):
        player_name(player_name), item_id(item_id), result() {}


void BuyCommand::execute(GameWorld& world) { result = world.buy_item(player_name, item_id); }


void BuyCommand::build_snapshot(SnapshotBuilder& builder) {
    if (result.status == BuyStatus::PLAYER_UNBOUNDED) {
        builder.add_action(ActionDTO(ChatMessageDTO(
                MessageType::ERROR, player_name,
                "Tenes que hablarle a un sacerdote o comerciante para poder comprar un item")));
        return;
    }

    static std::map<AllyType, std::string> ally_type_to_string({
            {AllyType::PRIEST, "Sacerdote"},
            {AllyType::MERCHANT, "Comerciante"},
    });

    if (not ally_type_to_string.contains(result.ally)) {
        throw std::runtime_error("BuyCommand recibió un NPC aliado desconocido");
    }

    static std::map<BuyStatus, std::string> result_to_message({
            {BuyStatus::ITEM_SOLD, "Gracias por tu compra!"},
            {BuyStatus::ITEM_NOT_SOLD, "Yo no vendo ese item!"},
            {BuyStatus::NOT_ENOUGH_GOLD, "No te alcanza el oro para comprar ese item!"},
    });

    if (not result_to_message.contains(result.status)) {
        throw std::runtime_error("BuyCommand recibió un resultado incorrecto");
    }

    builder.add_action(ActionDTO(ChatMessageDTO(MessageType::ALLY, ally_type_to_string.at(result.ally),
                                                player_name, result_to_message.at(result.status))));
}
