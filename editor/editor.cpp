#include "editor.h"

#include <ui_mapcanvas.h>

#include "ui_editor.h"

Editor::Editor(QWidget* parent):
    QMainWindow(parent), ui(new Ui::Editor)
{
    ui->setupUi(this);

    AssetData tile0 = {0, QPixmap(DATA_PATH "/tiles/0.png"), "Agua", ImageType::TILE, 4, 4, {{true, true, true, true}, {true, true, true, true}, {true, true, true, true}, {true, true, true, true}}};
    AssetData tile1 = {1, QPixmap(DATA_PATH "/tiles/1.png"), "Arena", ImageType::TILE, 4, 4, {{false, false, false, false}, {false, false, false, false}, {false, false, false, false}, {false, false, false, false}}};
    AssetData tile2 = {2, QPixmap(DATA_PATH "/tiles/2.png"), "Pasto", ImageType::TILE, 4,4, {{false, false, false, false}, {false, false, false, false}, {false, false, false, false}, {false, false, false, false}}};

    AssetData coll1 = {0, QPixmap(DATA_PATH "/collidable/0.png"), "Carreta", ImageType::COLLIDER, 3, 2,
            {{true,true,true},{true,true,true}}};

    tiles.insert({{tile0.id, tile0}, {tile1.id, tile1}, {tile2.id, tile2}});
    collidables.insert({{coll1.id, coll1}});

    map_canvas = new MapCanvas(tiles, collidables);
    ui->mapWidget->addWidget(map_canvas);

    asset_selector = new AssetSelector(tiles, collidables);
    ui->selectorWidget->addWidget(asset_selector);

    // Conexión botones
    connect(ui->eraseBtn, &QPushButton::clicked, this, &Editor::setEraseMode);

    // Conxiones Selector <-> Mapa (canvas)
    connect(asset_selector, &AssetSelector::clickedImage, map_canvas, &MapCanvas::setSelectedAsset);

    // Conexiones Mapa (canvas) <-> Mapa (logico)
}


void Editor::setEraseMode() const {
    map_canvas->setMode(EditorMode::ERASE);
}

Editor::~Editor() { delete ui; }
