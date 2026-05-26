target_sources(taller_editor
    PRIVATE
        main.cpp
        editor.cpp
        map_canvas.cpp
        editor_map.cpp
        image_selector.cpp

    PUBLIC
        components.h

        editor.h
        map_canvas.h
        editor_map.h
        image_selector.h

        UI/mapcanvas.ui
        UI/editor.ui
        UI/imageselector.ui
)
