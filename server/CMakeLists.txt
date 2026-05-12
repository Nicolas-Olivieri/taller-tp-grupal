target_sources(taller_server
    PRIVATE
        # .cpp files
        main.cpp
        server.cpp
        server_protocol.cpp
    PUBLIC
        # .h files
        server.h
        server_protocol.h
    )
