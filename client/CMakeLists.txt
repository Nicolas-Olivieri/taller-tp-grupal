target_sources(taller_client
    PRIVATE
        # .cpp files
        main.cpp
        client.cpp
        client_protocol.cpp

        SDL/client_game.cpp
        SDL/sprites/animation.cpp
        SDL/sprites/animation_pool.cpp

        SDL/sprites/sprite.cpp
        SDL/sprites/sprite_layer.cpp
        SDL/sprites/sprite_creator.cpp
        SDL/sprites/texture_pool.cpp


    PUBLIC
        # .h files
        client.h
        client_protocol.h

        SDL/client_game.h
        SDL/sprites/animation.h
        SDL/sprites/animation_pool.h

        SDL/sprites/sprite.h
        SDL/sprites/sprite_layer.h
        SDL/sprites/sprite_creator.h
        SDL/sprites/texture_pool.h

)
