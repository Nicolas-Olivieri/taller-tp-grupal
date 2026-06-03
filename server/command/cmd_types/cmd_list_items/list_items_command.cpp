#include "list_items_command.h"

#include <format>
#include <map>
#include <vector>


ListItemsCommand::ListItemsCommand(const std::string& player_name):
        player_name(player_name), ally(""), result({}) {}


void ListItemsCommand::execute(GameWorld& world) { result = world.list_ally_items(player_name); }


void ListItemsCommand::build_snapshot(SnapshotBuilder& builder) {
    static std::map<AllyType, std::string> ally_type_to_string({
            {AllyType::PRIEST, "Sacerdote"},
            {AllyType::MERCHANT, "Comerciante"},
            {AllyType::BANKER, "Banquero"},
    });

    if (not ally_type_to_string.contains(result.ally)) {
        throw std::runtime_error("ListItems cmd no se ejecutó correctamente");
    }

    if (result.items.empty()) {
        builder.add_action(
                ActionDTO(ChatMessageDTO(MessageVisibility::PRIVATE, ally_type_to_string.at(result.ally),
                                         player_name, "No tengo items para mostrar en este momento")));
        return;
    }

    std::vector<std::string> lines;
    builder.add_action(
            ActionDTO(ChatMessageDTO(MessageVisibility::PRIVATE, ally_type_to_string.at(result.ally),
                                     player_name, "Tengo los siguientes items:")));
    for (const auto& [item_id, price]: result.items) {
        // TODO: Usar ItemMapper para traducir el item_id a su nombre real
        lines.push_back(std::format("    - {} - Precio: {} monedas de oro", item_id, price));
    }

    builder.add_action(ActionDTO(ChatListDTO(lines, player_name)));
}
