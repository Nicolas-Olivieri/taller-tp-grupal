target_sources(taller_server
    PRIVATE
        # .cpp files
        main.cpp
        server.cpp
        server_protocol.cpp
        command/move_command.cpp
        game/game_world.cpp
        game/player.cpp
        game/position.cpp
    PUBLIC
        # .h files
        server.h
        server_protocol.h
        command/command.h
        command/move_command.h
        game/game_world.cpp
        game/player.cpp
        game/position.cpp
    )
