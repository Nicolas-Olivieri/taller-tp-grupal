target_sources(taller_editor
    PRIVATE
        main.cpp
        editor.cpp
        map_data.cpp
        grid_range.cpp

        view/map_canvas.cpp
        view/asset_selector.cpp

        persistance/map_saver.cpp
        persistance/map_loader.cpp

        ui/mapcanvas.ui
        ui/editor.ui
        ui/assetselector.ui

        config/editor_config.cpp
        config/editor_config_data.cpp

    PUBLIC
        components.h

        editor.h
        map_data.h
        grid_range.h

        view/map_canvas.h
        view/asset_selector.h

        persistance/map_saver.h
        persistance/map_loader.h

        config/asset_parser.h

        config/editor_config.h
        config/editor_config_data.h
)
