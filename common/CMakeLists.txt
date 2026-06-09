target_sources(taller_common
    PRIVATE
        # .cpp files
        liberror.cpp
        resolver.cpp
        resolvererror.cpp
        socket.cpp
        thread/thread.cpp
        foo.cpp
        util/rate_timer.cpp
        logger.cpp
        dto/lobby/credentials.cpp

        dto/events/buy_event.cpp
        dto/events/event.cpp
        dto/events/moveevent.cpp
        dto/events/interact_event.cpp
        dto/events/command.cpp
        dto/events/chatevent.cpp
        dto/events/deposit_gold_event.cpp
        dto/events/deposit_item_event.cpp
        dto/events/drop_item_event.cpp
        dto/events/sell_event.cpp
        dto/events/unequip_item_event.cpp
        dto/events/use_item_event.cpp
        dto/events/withdraw_gold_event.cpp
        dto/events/withdraw_item_event.cpp

        dto/snapshot/snapshot.cpp
        dto/snapshot/actions/action.cpp
        dto/snapshot/actions/action_types/act_attack/attack.cpp
        dto/snapshot/actions/action_types/act_message/chatmessage.cpp
        dto/snapshot/actions/action_types/act_resurrection/resurrection.cpp
        dto/snapshot/actions/action_types/act_death/death.cpp
        dto/snapshot/actions/action_types/act_list/chat_list.cpp
        dto/snapshot/actions/action_types/act_list_bank/list_bank.cpp
        dto/snapshot/actions/action_types/act_list_items/list_items.cpp

        dto/snapshot/info/appearance.cpp
        dto/snapshot/info/playerinfo.cpp
        dto/snapshot/info/creatureinfo.cpp
        dto/snapshot/info/equipment_info.cpp
        dto/snapshot/info/inventory_info.cpp
        dto/snapshot/info/lootinfo.cpp

        dto/snapshot/map/asset_info.cpp
        dto/snapshot/map/client_map_data.cpp
        dto/snapshot/map/grid_matrix.cpp
        dto/snapshot/map/server_map_data.cpp

        dto/snapshot/snapshot_builder.cpp

        protocol/protocol.cpp
        protocol/serializer.cpp
        protocol/deserializer.cpp

        dto/snapshot/actions/action_types/act_despawn/despawn.cpp

        dto/lobby/ally_info.cpp

        dto/snapshot/info/player_stats.cpp

        util/string_utils.cpp
        util/toml_helper.cpp

        dto/lobby/existence.cpp
        dto/lobby/create_player.cpp

    PUBLIC
        # .h files
        liberror.h
        resolver.h
        resolvererror.h
        socket.h
        thread/thread.h
        foo.h
        toml_test.h
        util/rate_timer.h
        direction.h
        queue.h
        logger.h
        dto/message.h
        dto/lobby/credentials.h

        dto/events/buy_event.h
        dto/events/event.h
        dto/events/moveevent.h
        dto/events/interact_event.h
        dto/events/command.h
        dto/events/chatevent.h
        dto/events/deposit_gold_event.h
        dto/events/deposit_item_event.h
        dto/events/drop_item_event.h
        dto/events/sell_event.h
        dto/events/unequip_item_event.h
        dto/events/use_item_event.h
        dto/events/withdraw_gold_event.h
        dto/events/withdraw_item_event.h

        dto/snapshot/snapshot.h
        dto/snapshot/actions/action.h
        dto/snapshot/actions/action_types/act_attack/attack.h
        dto/snapshot/actions/action_types/act_message/chatmessage.h
        dto/snapshot/actions/action_types/act_resurrection/resurrection.h
        dto/snapshot/actions/action_types/act_death/death.h
        dto/snapshot/actions/action_types/act_list/chat_list.h
        dto/snapshot/actions/action_types/act_list_bank/list_bank.h
        dto/snapshot/actions/action_types/act_list_items/list_items.h

        dto/snapshot/info/appearance.h
        dto/snapshot/info/playerinfo.h
        dto/snapshot/info/creatureinfo.h
        dto/snapshot/info/equipment_info.h
        dto/snapshot/info/inventory_info.h
        dto/snapshot/info/lootinfo.h

        dto/snapshot/map/asset_info.h
        dto/snapshot/map/client_map_data.h
        dto/snapshot/map/grid_matrix.h
        dto/snapshot/map/server_map_data.h

        dto/snapshot/snapshot_builder.h

        protocol/protocol.h
        protocol/serializer.h
        protocol/deserializer.h

        dto/snapshot/actions/action_types/act_despawn/despawn.h

        dto/lobby/ally_info.h
        npc_type.h

        dto/snapshot/info/player_stats.h

        util/string_utils.h
        util/toml_helper.h

        dto/lobby/existence.h
        dto/lobby/create_player.h
    )
