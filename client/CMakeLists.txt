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

        qt/login_window.cpp
        qt/creator_window.cpp
        qt/widgets/selector_widget.cpp
        qt/widgets/preview_widget.cpp

        connection/client_receiver.cpp
        connection/client_sender.cpp
        connection/connection_handler.cpp

        SDL/key_mapper.cpp
        SDL/user_interface.cpp

        config/client_config.cpp

    PUBLIC
        # .h files
        client.h
        client_constants.h

        TOML/animation_parser.h
        TOML/audio_parser.h

        SDL/client_game.h
        SDL/camera.h
        SDL/world.h

        SDL/sprites/animation.h
        SDL/sprites/animation_pool.h
        SDL/sprites/sprite.h
        SDL/sprites/sprite_layer.h
        SDL/sprites/sprite_creator.h
        SDL/sprites/texture_pool.h

        qt/login_window.h
        qt/ui/login_window.ui
        qt/creator_window.h
        qt/ui/creator_window.ui
        qt/widgets/selector_widget.h
        qt/ui/selector_widget.ui
        qt/widgets/preview_widget.h
        qt/ui/preview_widget.ui

        connection/client_receiver.h
        connection/client_sender.h
        connection/connection_handler.h

        SDL/key_mapper.h

        SDL/user_interface.h

        config/client_config.h
    )
