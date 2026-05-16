target_sources(taller_server
    PRIVATE
        # .cpp files
        main.cpp
        server.cpp
        protocol/server_protocol.cpp
    PUBLIC
        # .h files
        server.h
        protocol/server_protocol.h
    )
