target_sources(taller_editor
    PRIVATE
        main.cpp
        editor.cpp
        map_canvas.cpp
        editor_map.cpp
        asset_selector.cpp

    PUBLIC
        components.h

        editor.h
        map_canvas.h
        editor_map.h
        asset_selector.h

        mapcanvas.ui
        editor.ui
        assetselector.ui
)
