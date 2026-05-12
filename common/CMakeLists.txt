target_sources(taller_common
    PRIVATE
        # .cpp files
        toml_test.cpp
        liberror.cpp
        resolver.cpp
        resolvererror.cpp
        socket.cpp
        foo.cpp
    PUBLIC
        # .h files
        liberror.h
        resolver.h
        resolvererror.h
        socket.h
        types.h
        foo.h
    )
