target_sources(taller_server
    PRIVATE
        # .cpp files
        main.cpp
        server.cpp

        command/cmd_types/cmd_buy/buy_command.cpp
        command/cmd_types/cmd_chatmessage/chatmessage_command.cpp
        command/cmd_types/cmd_deposit_gold/deposit_gold_command.cpp
        command/cmd_types/cmd_deposit_item/deposit_item_command.cpp
        command/cmd_types/cmd_despawn/despawn_command.cpp
        command/cmd_types/cmd_heal/heal_command.cpp
        command/cmd_types/cmd_interact/interact_command.cpp
        command/cmd_types/cmd_list_items/list_items_command.cpp
        command/cmd_types/cmd_move/move_command.cpp
        command/cmd_types/cmd_resurrect/resurrect_command.cpp
        command/cmd_types/cmd_sell/sell_command.cpp
        command/cmd_types/cmd_spawn/spawn_command.cpp
        command/cmd_types/cmd_withdraw_gold/withdraw_gold_command.cpp
        command/cmd_types/cmd_withdraw_item/withdraw_item_command.cpp

        command/cmd_results/ally_execute/ally_execute_result.cpp
        command/cmd_results/interact/interact_result.cpp

        command/command_factory.cpp

        game/game_world.cpp
        game/player/player.cpp
        game/stats/recoverable_stat.cpp
        game/stats/mana.cpp
        game/stats/health.cpp
        game/stats/experience.cpp
        game/stats/stats.cpp
        util/calculator.cpp
        game/position.cpp
        game/grid.cpp
        game/killable.cpp
        game/loot.cpp
        game/tile.cpp

        game/creatures/creature.cpp
        game/creatures/updatestatus.cpp
        game/creatures/state/idlestate.cpp
        game/creatures/state/followingstate.cpp
        game/creatures/state/attackingstate.cpp

        connection/acceptor.cpp
        connection/lobbyhandler.cpp
        connection/game_acceptor.cpp
        game/game_loop.cpp
        connection/client_handler.cpp
        connection/sender.cpp
        connection/receiver.cpp
        connection/event_broadcaster.cpp
        connection/broadcast_register.cpp
        persistance/playerdata.cpp
        persistance/playerdatabase.cpp
        persistance/playerindex.cpp
        persistance/playerrepository.cpp
        config/game_config.cpp

        game/player/bank/bank.cpp
        game/player/inventory/inventory.cpp
        game/player/inventory/item_mapper.cpp
        game/player/inventory/goldmanager.cpp

        game/allies/ally.cpp
        game/allies/ally_action_payload.cpp
        game/allies/banker.cpp
        game/allies/merchant.cpp
        game/allies/priest.cpp
        game/allies/vendor_ally.cpp

    PUBLIC
        # .h files
        server.h

        command/cmd_types/cmd_buy/buy_command.h
        command/cmd_types/cmd_chatmessage/chatmessage_command.h
        command/cmd_types/cmd_deposit_gold/deposit_gold_command.h
        command/cmd_types/cmd_deposit_item/deposit_item_command.h
        command/cmd_types/cmd_despawn/despawn_command.h
        command/cmd_types/cmd_heal/heal_command.h
        command/cmd_types/cmd_interact/interact_command.h
        command/cmd_types/cmd_list_items/list_items_command.h
        command/cmd_types/cmd_move/move_command.h
        command/cmd_types/cmd_sell/sell_command.h
        command/cmd_types/cmd_spawn/spawn_command.h
        command/cmd_types/cmd_withdraw_gold/withdraw_gold_command.h
        command/cmd_types/cmd_withdraw_item/withdraw_item_command.h
        command/cmd_types/cmd_resurrect/resurrect_command.h

        command/cmd_results/ally_execute/buy_result.h
        command/cmd_results/ally_execute/deposit_gold_result.h
        command/cmd_results/ally_execute/deposit_item_result.h
        command/cmd_results/ally_execute/heal_result.h
        command/cmd_results/ally_execute/list_bank_result.h
        command/cmd_results/ally_execute/list_items_result.h
        command/cmd_results/ally_execute/list_outcomes.h
        command/cmd_results/ally_execute/resurrect_result.h
        command/cmd_results/ally_execute/sell_result.h
        command/cmd_results/ally_execute/withdraw_gold_result.h
        command/cmd_results/ally_execute/withdraw_item_result.h
        command/cmd_results/ally_execute/ally_execute_result.h

        command/cmd_results/interact/attack_result.h
        command/cmd_results/interact/bind_result.h
        command/cmd_results/interact/interact_result.h

        command/command.h
        command/command_factory.h

        game/game_world.h
        game/player/player.h
        game/stats/recoverable_stat.h
        game/stats/mana.h
        game/stats/health.h
        game/stats/experience.h
        game/stats/stats.h
        util/calculator.h
        game/position.h
        game/grid.h
        game/interactive.h
        game/killable.h
        game/loot.h
        game/tile.h

        game/creatures/creature.h
        game/creatures/updatestatus.h
        game/creatures/state/creaturestate.h
        game/creatures/state/idlestate.h
        game/creatures/state/followingstate.h
        game/creatures/state/attackingstate.h

        connection/acceptor.h
        connection/lobbyhandler.h
        connection/connectioninfo.h
        connection/game_acceptor.h
        game/game_loop.h
        connection/client_handler.h
        connection/sender.h
        connection/receiver.h
        connection/event_broadcaster.h
        connection/broadcast_register.h
        persistance/playerdata.h
        persistance/playerdatabase.h
        persistance/playerindex.h
        persistance/playerrepository.h
        config/game_config.h

        game/player/bank/bank.h
        game/player/inventory/inventory.h
        game/equipment.h
        game/player/inventory/item_mapper.h
        game/player/inventory/goldmanager.h

        game/attacker.h

        game/allies/ally.h
        game/allies/ally_action_payload.h
        game/allies/banker.h
        game/allies/merchant.h
        game/allies/priest.h
        game/allies/vendor_ally.h

        game/world_update_status.h
    )
