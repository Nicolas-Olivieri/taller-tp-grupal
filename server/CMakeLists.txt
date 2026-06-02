target_sources(taller_server
    PRIVATE
        # .cpp files
        main.cpp
        server.cpp

        command/cmd_types/cmd_chatmessage/chatmessage_command.cpp
        command/cmd_types/cmd_despawn/despawn_command.cpp
        command/cmd_types/cmd_heal/heal_command.cpp
        command/cmd_types/cmd_interact/interact_command.cpp
        command/cmd_types/cmd_move/move_command.cpp
        command/cmd_types/cmd_resurrect/resurrect_command.cpp
        command/cmd_types/cmd_spawn/spawn_command.cpp
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

        game/player/inventory/inventory.cpp
        game/player/inventory/item_mapper.cpp

        game/allies/ally.cpp
        game/allies/priest.cpp
    PUBLIC
        # .h files
        server.h

        command/cmd_types/cmd_chatmessage/chatmessage_command.h
        command/cmd_types/cmd_despawn/despawn_command.h
        command/cmd_types/cmd_heal/heal_command.h
        command/cmd_types/cmd_interact/interact_command.h
        command/cmd_types/cmd_move/move_command.h
        command/cmd_types/cmd_resurrect/resurrect_command.h
        command/cmd_types/cmd_spawn/spawn_command.h
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

        game/player/inventory/inventory.h
        game/player/inventory/equipment.h
        game/player/inventory/item_mapper.h
        game/attacker.h

        game/allies/ally.h
        game/allies/priest.h
    )
