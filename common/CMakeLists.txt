target_sources(taller_common
    PRIVATE
        # .cpp files
        liberror.cpp
        resolver.cpp
        resolvererror.cpp
        socket.cpp
        thread/thread.cpp
        foo.cpp
        logger.cpp
    PUBLIC
        # .h files
        liberror.h
        resolver.h
        resolvererror.h
        socket.h
        thread/thread.h
        types.h
        foo.h
        toml_test.h
        logger.h
        message.h
        dto/credentials.h
    )
