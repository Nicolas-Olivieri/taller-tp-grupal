#ifndef EDITOR_H
#define EDITOR_H

#include <QMainWindow>
#include <QPushButton>
#include <string>

#include "persistance/map_loader.h"
#include "persistance/map_saver.h"
#include "toml11/types.hpp"
#include "view/asset_selector.h"
#include "view/map_canvas.h"

#include "components.h"
#include "map_data.h"

namespace Ui {
class Editor;
}

class Editor: public QMainWindow {
    Q_OBJECT

public:
    explicit Editor(QWidget* parent = nullptr);

    ~Editor();

private slots:
    void set_mode(const EditorMode& new_mode);

    void prompt_file_saving();

    void prompt_file_opening();

private:
    Ui::Editor* ui;
    QHash<uint8_t, AssetData> tiles;
    QHash<uint8_t, AssetData> colliders;
    QHash<uint8_t, AssetData> npcs;

    MapData map_data;
    MapCanvas map_canvas;
    AssetSelector asset_selector;
    QHash<EditorMode, QPushButton*> action_buttons;

    MapLoader loader;
    MapSaver saver;

    QHash<uint8_t, AssetData> populate_hash(const std::string& category_name, ImageType type);
};

#endif  // EDITOR_H
