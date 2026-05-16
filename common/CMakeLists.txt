target_sources(taller_common
    PRIVATE
        # .cpp files
        liberror.cpp
        resolver.cpp
        resolvererror.cpp
        socket.cpp
        foo.cpp

        util/rate_timer.cpp

    PUBLIC
        # .h files
        liberror.h
        resolver.h
        resolvererror.h
        socket.h
        types.h
        foo.h
        toml_test.h

        util/rate_timer.h
)
