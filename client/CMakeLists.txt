target_sources(taller_client
    PRIVATE
        # .cpp files
        main.cpp
        client.cpp
        client_protocol.cpp

        SDL/animation.cpp
        SDL/client_game.cpp
        SDL/sprite.cpp
        SDL/rate_timer.cpp

    PUBLIC
        # .h files
        client.h
        client_protocol.h

        SDL/animation.h
        SDL/client_game.h
        SDL/sprite.h
        SDL/rate_timer.h
)
