#ifndef EDITOR_H
#define EDITOR_H

#include <QMainWindow>

#include "components.h"
#include "map_data.h"
#include "asset_selector.h"
#include "map_canvas.h"
#include "map_saver.h"

namespace Ui {
class Editor;
}

class Editor: public QMainWindow {
    Q_OBJECT

public:
    explicit Editor(QWidget* parent = nullptr);

    ~Editor();
    
private slots:
    void set_mode(const EditorMode& new_mode) const;

    void prompt_file_saving();

private:
    Ui::Editor* ui;
    QHash<uint8_t, AssetData> tiles;
    QHash<uint8_t, AssetData> collidables;

    MapData map_data;
    MapCanvas* map_canvas;
    AssetSelector* asset_selector;
    QHash<EditorMode, QPushButton*> action_buttons;

    MapSaver saver;
};

#endif  // EDITOR_H
