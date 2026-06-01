#include "editor.h"

#include <QFileDialog>
#include <ui_mapcanvas.h>

#include "ui_editor.h"

Editor::Editor(QWidget* parent):
    QMainWindow(parent), ui(new Ui::Editor), map_data(MapData()), map_canvas(MapCanvas(this->map_data)),
    loader(this->map_data, this->map_canvas, tiles, colliders), saver(MapSaver(this->map_data))
{
    ui->setupUi(this);

    AssetData tile0 = {0, QPixmap(DATA_PATH "/tiles/0.png"), "Agua", ImageType::TILE, 4, 4, QRect(0,0,4,4)};
    AssetData tile1 = {1, QPixmap(DATA_PATH "/tiles/1.png"), "Arena", ImageType::TILE, 4, 4, QRect(0,0,0,0)};
    AssetData tile2 = {2, QPixmap(DATA_PATH "/tiles/2.png"), "Pasto", ImageType::TILE, 4,4, QRect(0,0,0,0)};

    AssetData coll1 = {0, QPixmap(DATA_PATH "/collidable/0.png"), "Carreta", ImageType::COLLIDER, 3, 2, QRect(0,0,3,2)};

    tiles.insert({{tile0.id, tile0}, {tile1.id, tile1}, {tile2.id, tile2}});
    colliders.insert({{coll1.id, coll1}});


    ui->mapWidget->addWidget(&map_canvas);

    asset_selector = new AssetSelector(tiles, colliders);
    ui->selectorWidget->addWidget(asset_selector);

    action_buttons.insert({ {EditorMode::DRAW, ui->drawBtn},
                                    {EditorMode::ERASE, ui->eraseBtn},
                                    {EditorMode::DRAG, ui->dragBtn}});


    // Conexión botones
    connect(ui->saveBtn, &QPushButton::clicked, this, &Editor::prompt_file_saving);
    connect(ui->loadBtn, &QPushButton::clicked, this, &Editor::prompt_file_opening);
    connect(ui->cbox_unwalkables, &QCheckBox::clicked, &map_canvas, &MapCanvas::set_visibility_unwalkables);

    for (const auto& [mode, btn] : action_buttons.asKeyValueRange()) {
        connect(btn, &QPushButton::clicked, this, [this, mode] {set_mode(mode);});
    }

    // Conxiones Selector <-> Mapa (canvas)
    connect(asset_selector, &AssetSelector::clickedImage, &map_canvas, &MapCanvas::set_selected_asset);
}

void Editor::set_mode(const EditorMode& new_mode) {
    for (const auto& [mode, btn] : action_buttons.asKeyValueRange()) {
        btn->setChecked(mode == new_mode);
    }
    map_canvas.set_mode(new_mode);
}

void Editor::prompt_file_saving() {
    const QString filename = QFileDialog::getSaveFileName();
    saver.save(filename);
}

void Editor::prompt_file_opening() {
    set_mode(EditorMode::DRAW);
    const QString fileName = QFileDialog::getOpenFileName(this, tr("Cargar mapa"),
                                                            QDir::currentPath(),tr("bin files (*.bin)"));
    loader.load(fileName);
    set_mode(EditorMode::DRAG);
}

Editor::~Editor() { delete ui; }
