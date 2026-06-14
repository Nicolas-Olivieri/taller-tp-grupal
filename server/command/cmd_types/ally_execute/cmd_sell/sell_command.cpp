#include "sell_command.h"

#include <map>


SellCommand::SellCommand(const std::string& player_name, const uint8_t item_id):
        player_name(player_name), item_id(item_id), result() {}


void SellCommand::execute(GameWorld& world) { result = world.sell_item(player_name, item_id); }


void SellCommand::build_snapshot(SnapshotBuilder& builder) {
    if (result.status == SellStatus::PLAYER_UNBOUNDED) {
        const std::string& error_message = "Hablale a un comerciante para poder vender un ítem";
        builder.add_action(ActionDTO(ChatMessageDTO(MessageType::ERROR, player_name, error_message)));
        return;
    }

    static std::map<AllyType, std::string> ally_type_to_string({
            {AllyType::PRIEST, "Sacerdote"},
            {AllyType::MERCHANT, "Comerciante"},
            {AllyType::BANKER, "Banquero"},
    });

    if (not ally_type_to_string.contains(result.ally)) {
        throw std::runtime_error("SellCommand recibió un NPC aliado desconocido");
    }

    static std::map<SellStatus, std::string> result_to_message({
            {SellStatus::ITEM_BOUGHT, "Un placer hacer negocios"},
            {SellStatus::ITEM_NOT_OWNED, "Estás tratando de vender un ítem que no tenés en tu inventario"},
            {SellStatus::ITEM_EQUIPPED, "Estás tratando de vender un ítem que tenés equipado"},
            {SellStatus::GHOST_FAIL, "Perdón, no le compro a fantasmas"},
            {SellStatus::ACTION_NOT_ACCEPTED, "Perdón, yo no puedo hacer eso"},
    });

    if (not result_to_message.contains(result.status)) {
        throw std::runtime_error("SellCommand recibió un resultado incorrecto");
    }

    const std::string& sender = ally_type_to_string.at(result.ally);
    const std::string& content = result_to_message.at(result.status);
    builder.add_action(ActionDTO(ChatMessageDTO(MessageType::ALLY, sender, player_name, content)));
}
