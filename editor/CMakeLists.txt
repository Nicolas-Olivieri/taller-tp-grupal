target_sources(taller_editor
    PRIVATE
        main.cpp
        editor.cpp
        map_canvas.cpp
        editor_map.cpp
        imageselector.cpp

    PUBLIC
        components.h

        editor.h
        map_canvas.h
        editor_map.h
        imageselector.h

        mapcanvas.ui
        editor.ui
        imageselector.ui
)
