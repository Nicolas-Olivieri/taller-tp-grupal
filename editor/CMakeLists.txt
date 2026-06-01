target_sources(taller_editor
    PRIVATE
        main.cpp
        editor.cpp
        map_canvas.cpp
        map_data.cpp
        asset_selector.cpp
        map_saver.cpp
        map_loader.cpp

    PUBLIC
        components.h

        editor.h
        map_canvas.h
        map_data.h
        asset_selector.h
        map_saver.h
        map_loader.h

        mapcanvas.ui
        editor.ui
        assetselector.ui
)
