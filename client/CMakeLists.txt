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

        SDL/sprites/base/sprite.cpp
        SDL/sprites/base/sprite_layer.cpp
        SDL/sprites/base/sprite_label.cpp
        SDL/sprites/base/world_sprite.cpp
        SDL/sprites/fixed/effect_sprite.cpp
        SDL/sprites/fixed/fixed_sprite.cpp
        SDL/sprites/moving/moving_sprite.cpp
        SDL/sprites/moving/enemy_sprite.cpp
        SDL/sprites/moving/player_sprite.cpp
        SDL/sprites/ui/progress_bar_sprite.cpp
        SDL/sprites/ui/hud_sprite.cpp
        SDL/sprites/ui/text_sprite.cpp
        SDL/sprites/ui/interface_sprite.cpp

        SDL/sprite_creation/animation.cpp
        SDL/sprite_creation/animation_pool.cpp
        SDL/sprite_creation/sprite_creator.cpp
        SDL/sprite_creation/texture_pool.cpp

        qt/login_window.cpp
        qt/creator_window.cpp
        qt/widgets/selector_widget.cpp
        qt/widgets/preview_widget.cpp
        qt/widgets/resolution_widget.cpp

        connection/client_receiver.cpp
        connection/client_sender.cpp
        connection/connection_handler.cpp

        SDL/user_interface.cpp
        SDL/ui/inventory_ui.cpp
        SDL/ui/chat_box_ui.cpp
        SDL/ui/help_box_ui.cpp

        config/client_config.cpp

        util/key_mapper.cpp
        util/command_handler.cpp

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


        SDL/sprites/base/sprite.h
        SDL/sprites/base/sprite_label.h
        SDL/sprites/base/sprite_layer.h
        SDL/sprites/base/world_sprite.h
        SDL/sprites/fixed/effect_sprite.h
        SDL/sprites/fixed/fixed_sprite.h
        SDL/sprites/moving/moving_sprite.h
        SDL/sprites/moving/enemy_sprite.h
        SDL/sprites/moving/player_sprite.h
        SDL/sprites/ui/progress_bar_sprite.h
        SDL/sprites/ui/hud_sprite.h
        SDL/sprites/ui/text_sprite.h
        SDL/sprites/ui/interface_sprite.h

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
        qt/widgets/resolution_widget.cpp
        qt/ui/resolution_widget.ui

        connection/client_receiver.h
        connection/client_sender.h
        connection/connection_handler.h


        SDL/user_interface.h
        SDL/ui/inventory_ui.h
        SDL/ui/chat_box_ui.h
        SDL/ui/help_box_ui.h

        config/client_config.h
        config/client_data.h

        util/key_mapper.h
        util/command_handler.h
    )
