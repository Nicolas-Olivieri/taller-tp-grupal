target_sources(taller_client
    PRIVATE
        # .cpp files
        main.cpp
        client.cpp
        protocol/client_protocol.cpp
    PUBLIC
        # .h files
        client.h
        protocol/client_protocol.h
    )
