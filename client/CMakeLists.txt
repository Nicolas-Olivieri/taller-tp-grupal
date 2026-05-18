target_sources(taller_client
    PRIVATE
        # .cpp files
        main.cpp
        client.cpp
        qt/lobby.cpp
        client_game.cpp
    PUBLIC
        # .h files
        client.h
        qt/lobby.h
        client_game.h
    )
