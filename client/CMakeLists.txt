target_sources(taller_client
    PRIVATE
        # .cpp files
        main.cpp
        client.cpp

        SDL/client_game.cpp
        SDL/camera.cpp
        SDL/world.cpp

        SDL/sprites/animation.cpp
        SDL/sprites/animation_pool.cpp
        SDL/sprites/sprite.cpp
        SDL/sprites/sprite_layer.cpp
        SDL/sprites/sprite_creator.cpp
        SDL/sprites/texture_pool.cpp

        qt/lobby.cpp
        qt/lobbywindow.cpp

        connection/client_receiver.cpp
        connection/client_sender.cpp
        connection/connection_handler.cpp

        SDL/key_mapper.cpp
        SDL/user_interface.cpp

    PUBLIC
        # .h files
        client.h
        client_constants.h

        TOML/animation_parser.h

        SDL/client_game.h
        SDL/camera.h
        SDL/world.h

        SDL/sprites/animation.h
        SDL/sprites/animation_pool.h
        SDL/sprites/sprite.h
        SDL/sprites/sprite_layer.h
        SDL/sprites/sprite_creator.h
        SDL/sprites/texture_pool.h

        qt/lobby.h
        qt/lobbywindow.h
        qt/lobbywindow.ui

        connection/client_receiver.h
        connection/client_sender.h
        connection/connection_handler.h

        SDL/key_mapper.h

        SDL/user_interface.cpp
    )
