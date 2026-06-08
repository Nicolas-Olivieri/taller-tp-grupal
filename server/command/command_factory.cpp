#include "command_factory.h"

#include "cmd_types/cmd_buy/buy_command.h"
#include "cmd_types/cmd_chatmessage/chatmessage_command.h"
#include "cmd_types/cmd_deposit_gold/deposit_gold_command.h"
#include "cmd_types/cmd_deposit_item/deposit_item_command.h"
#include "cmd_types/cmd_heal/heal_command.h"
#include "cmd_types/cmd_interact/interact_command.h"
#include "cmd_types/cmd_list_items/list_items_command.h"
#include "cmd_types/cmd_move/move_command.h"
#include "cmd_types/cmd_pickup/pickup_command.h"
#include "cmd_types/cmd_resurrect/resurrect_command.h"
#include "cmd_types/cmd_sell/sell_command.h"
#include "cmd_types/cmd_use_item/use_item_command.h"
#include "cmd_types/cmd_withdraw_gold/withdraw_gold_command.h"
#include "cmd_types/cmd_withdraw_item/withdraw_item_command.h"


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

        case CommandType::DEPOSIT_ITEM:
            return std::make_unique<DepositItemCommand>(player_name, dto.item_id);

        case CommandType::WITHDRAW_ITEM:
            return std::make_unique<WithdrawItemCommand>(player_name, dto.item_id);

        case CommandType::DEPOSIT_GOLD:
            return std::make_unique<DepositGoldCommand>(player_name, dto.gold_amount);

        case CommandType::WITHDRAW_GOLD:
            return std::make_unique<WithdrawGoldCommand>(player_name, dto.gold_amount);

        case CommandType::PICKUP:
            return std::make_unique<PickUpCommand>(player_name, dto.x, dto.y);

        case CommandType::USE_ITEM:
            return std::make_unique<UseItemCommand>(player_name, dto.item_id);

        default:
            throw std::invalid_argument("CommandFactory recibió un comando desconocido");
    }
}
