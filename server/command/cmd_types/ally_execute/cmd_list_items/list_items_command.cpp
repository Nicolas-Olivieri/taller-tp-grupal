#include "list_items_command.h"

#include <map>

#include "common/dto/snapshot/actions/action_types/act_list_bank/list_bank.h"
#include "server/command/cmd_results/ally_execute/list/outcomes/player_unbounded/player_unbounded_outcome.h"


ListItemsCommand::ListItemsCommand(const std::string& player_name): player_name(player_name), result() {}


void ListItemsCommand::execute(GameWorld& world) { result = world.list_ally_items(player_name); }


void ListItemsCommand::build_snapshot(SnapshotBuilder& builder) {
    if (dynamic_cast<PlayerUnboundOutcome*>(result.get())) {
        const std::string& error_message = "Hablale a un NPC aliado para pedirle su lista de ítems";
        builder.add_action(ActionDTO(ChatMessageDTO(MessageType::ERROR, player_name, error_message)));
        return;
    }

    static std::map<AllyType, std::string> ally_type_to_string({
            {AllyType::PRIEST, "Sacerdote"},
            {AllyType::MERCHANT, "Comerciante"},
            {AllyType::BANKER, "Banquero"},
    });

    if (not ally_type_to_string.contains(result.get()->ally)) {
        throw std::runtime_error("ListItemsCommand recibió un NPC aliado desconocido");
    }

    if (const auto bank_outcome = dynamic_cast<BankVaultOutcome*>(result.get())) {
        handle_bank_outcome(builder, *bank_outcome);
        return;
    }

    if (const auto vendor_outcome = dynamic_cast<VendorListOutcome*>(result.get())) {
        handle_vendor_outcome(builder, *vendor_outcome);
        return;
    }

    throw std::runtime_error("ListItemsCommand recibió un resultado incorrecto");
}


void ListItemsCommand::handle_bank_outcome(SnapshotBuilder& builder,
                                           const BankVaultOutcome& bank_outcome) const {
    builder.add_action(ActionDTO(
            ChatMessageDTO(MessageType::ALLY, "Banquero", player_name, "Bienvenido! En el banco tenés:")));
    builder.add_action(
            ActionDTO(ListBankDTO(MessageType::ALLY, bank_outcome.gold, bank_outcome.items, player_name)));
}


void ListItemsCommand::handle_vendor_outcome(SnapshotBuilder& builder,
                                             const VendorListOutcome& vendor_outcome) const {
    static std::map<AllyType, std::string> ally_type_to_string({
            {AllyType::PRIEST, "Sacerdote"},
            {AllyType::MERCHANT, "Comerciante"},
    });

    if (vendor_outcome.items.empty()) {
        builder.add_action(
                ActionDTO(ChatMessageDTO(MessageType::ALLY, ally_type_to_string.at(vendor_outcome.ally),
                                         player_name, "No tengo ítems para mostrar en este momento")));
        return;
    }

    builder.add_action(
            ActionDTO(ChatMessageDTO(MessageType::ALLY, ally_type_to_string.at(vendor_outcome.ally),
                                     player_name, "Tengo los siguientes ítems:")));
    builder.add_action(ActionDTO(ListItemsDTO(MessageType::ALLY, vendor_outcome.items, player_name)));
}
