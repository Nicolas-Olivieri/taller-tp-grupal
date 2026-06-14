target_sources(taller_client
    PRIVATE
        # .cpp files
        main.cpp
        client.cpp

        SDL/client_game.cpp
        SDL/camera.cpp
        SDL/world.cpp

        SDL/audio/audio_manager.cpp
        SDL/audio/sound_pool.cpp

        SDL/fonts/font_manager.cpp

        SDL/sprites/sprite.cpp
        SDL/sprites/effect_sprite.cpp
        SDL/sprites/sprite_layer.cpp
        SDL/sprites/moving_sprite.cpp
        SDL/sprites/fixed_sprite.cpp
        SDL/sprites/enemy_sprite.cpp
        SDL/sprites/player_sprite.cpp
        SDL/sprites/sprite_label.cpp

        SDL/sprite_creation/animation.cpp
        SDL/sprite_creation/animation_pool.cpp
        SDL/sprite_creation/sprite_creator.cpp
        SDL/sprite_creation/texture_pool.cpp

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
        TOML/font_parser.h

        SDL/client_game.h
        SDL/camera.h
        SDL/world.h

        SDL/audio/audio_manager.h
        SDL/audio/sound_pool.h

        SDL/fonts/font_manager.h


        SDL/sprites/sprite.h
        SDL/sprites/effect_sprite.h
        SDL/sprites/sprite_label.h
        SDL/sprites/sprite_layer.h
        SDL/sprites/moving_sprite.h
        SDL/sprites/fixed_sprite.h
        SDL/sprites/enemy_sprite.h
        SDL/sprites/player_sprite.h

        SDL/sprite_creation/animation.h
        SDL/sprite_creation/animation_pool.h
        SDL/sprite_creation/sprite_creator.h
        SDL/sprite_creation/texture_pool.h

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
