#ifndef EDITOR_H
#define EDITOR_H

#include <QMainWindow>

#include "components.h"
#include "editor_map.h"
#include "asset_selector.h"
#include "map_canvas.h"

namespace Ui {
class Editor;
}

class Editor: public QMainWindow {
    Q_OBJECT

public:
    explicit Editor(QWidget* parent = nullptr);

    ~Editor();
    
private slots:
    void setEraseMode() const;
    // void saveMap();

private:
    Ui::Editor* ui;
    // EditorMap map;
    QHash<uint8_t, AssetData> tiles;
    QHash<uint8_t, AssetData> collidables;

    MapCanvas* map_canvas;
    AssetSelector* asset_selector;
};

#endif  // EDITOR_H
