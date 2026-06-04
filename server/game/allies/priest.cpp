#include "priest.h"

#include <algorithm>
#include <map>

#include "server/game/player/inventory/item_mapper.h"
#include "server/game/player/player.h"


Priest::Priest(const Position& position):
        VendorAlly(position, AllyType::PRIEST, ItemMapper::get_random_priest_items()) {}


AllyExecuteResult Priest::execute(Player& player, const AllyActionPayload& payload) const {
    switch (payload.action) {
        case AllyAction::HEAL:
            return handle_heal(player);

        case AllyAction::RESURRECT:
            return handle_resurrect(player);

        case AllyAction::LIST_ITEMS:
            return handle_list_items();

        case AllyAction::BUY:
            return handle_buy_item(player, payload.item_id);

        case AllyAction::SELL:
            return handle_sell_item();

        default:
            break;
    }

    return AllyExecuteResult(true);
}


AllyExecuteResult Priest::handle_heal(Player& player) const {
    if (player.is_alive()) {
        std::cout << "[Priest] El jugador fue curado" << std::endl;
        player.heal();
        return AllyExecuteResult(HealResult::PLAYER_HEALED);
    }

    std::cout << "[Priest] El jugador está muerto" << std::endl;
    return AllyExecuteResult(HealResult::PLAYER_IS_DEAD);
}


AllyExecuteResult Priest::handle_resurrect(Player& player) const {
    if (not player.is_alive()) {
        std::cout << "[Priest] El jugador fue resucitado" << std::endl;
        player.heal();
        return AllyExecuteResult(ResurrectResult::PLAYER_RESURRECTED);
    }

    std::cout << "[Priest] El jugador ya está vivo" << std::endl;
    return AllyExecuteResult(ResurrectResult::PLAYER_IS_ALIVE);
}


AllyExecuteResult Priest::handle_sell_item() const {
    return AllyExecuteResult(SellResult(SellStatus::ITEM_NOT_ACCEPTED, type));
}
