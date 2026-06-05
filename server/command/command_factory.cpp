#include "command_factory.h"

#include "cmd_types/cmd_buy/buy_command.h"
#include "cmd_types/cmd_chatmessage/chatmessage_command.h"
#include "cmd_types/cmd_heal/heal_command.h"
#include "cmd_types/cmd_interact/interact_command.h"
#include "cmd_types/cmd_list_items/list_items_command.h"
#include "cmd_types/cmd_move/move_command.h"
#include "cmd_types/cmd_resurrect/resurrect_command.h"
#include "cmd_types/cmd_sell/sell_command.h"


CommandFactory::CommandFactory(const std::string& player_name): player_name(player_name) {}


std::unique_ptr<Command> CommandFactory::create(const RequestedCommandDTO& dto) const {
    switch (dto.command) {
        case CommandType::MOVE:
            return std::make_unique<MoveCommand>(player_name, dto.direction);

        case CommandType::INTERACT:
            return std::make_unique<InteractCommand>(player_name, dto.x, dto.y);

        case CommandType::CHAT:
            return std::make_unique<ChatMessageCommand>(player_name, dto.receiver, dto.message);

        case CommandType::RESURRECT:
            return std::make_unique<ResurrectCommand>(player_name);

        case CommandType::HEAL:
            return std::make_unique<HealCommand>(player_name);

        case CommandType::LIST_ITEMS:
            return std::make_unique<ListItemsCommand>(player_name);

        case CommandType::BUY_ITEM:
            return std::make_unique<BuyCommand>(player_name, dto.item_id);

        case CommandType::SELL_ITEM:
            return std::make_unique<SellCommand>(player_name, dto.item_id);

        default:
            throw std::invalid_argument("CommandFactory recibió un comando desconocido");
    }
}
