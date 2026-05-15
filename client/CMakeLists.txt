target_sources(taller_client
    PRIVATE
        # .cpp files
        main.cpp
        client.cpp
        client_protocol.cpp
        qt/lobby.cpp
        client_game.cpp
    PUBLIC
        # .h files
        client.h
        client_protocol.h
        qt/lobby.h
        client_game.h
    )
