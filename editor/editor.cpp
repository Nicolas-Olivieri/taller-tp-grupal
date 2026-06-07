#include "editor.h"

#include <QCheckBox>
#include <QFileDialog>
#include <QMessageBox>
#include <string>
#include <vector>

#include "toml/asset_parser.h"

#include "toml.hpp"
#include "ui_editor.h"

#define TILE_SIZE 32

Editor::Editor(QWidget* parent):
        QMainWindow(parent),
        ui(new Ui::Editor),
        tiles(populate_hash("tiles", ImageType::TILE)),
        colliders(populate_hash("colliders", ImageType::COLLIDER)),
        npcs(populate_hash("npcs", ImageType::NPC)),
        map_data(MapData()),
        map_canvas(MapCanvas(this->map_data)),
        asset_selector(this->tiles, this->colliders, this->npcs),
        loader(this->map_data, this->map_canvas, tiles, colliders, npcs),
        saver(MapSaver(this->map_data)) {

    ui->setupUi(this);
    ui->mapWidget->addWidget(&map_canvas);
    ui->selectorWidget->addWidget(&asset_selector);

    action_buttons.insert({{EditorMode::DRAW, ui->drawBtn},
                           {EditorMode::ERASE, ui->eraseBtn},
                           {EditorMode::DRAG, ui->dragBtn}});

    // Conexión botones
    connect(ui->saveBtn, &QPushButton::clicked, this, &Editor::prompt_file_saving);
    connect(ui->loadBtn, &QPushButton::clicked, this, &Editor::prompt_file_opening);
    connect(ui->cbox_unwalkables, &QCheckBox::clicked, &map_canvas, &MapCanvas::set_visibility_unwalkables);

    for (const auto& [mode, btn]: action_buttons.asKeyValueRange()) {
        connect(btn, &QPushButton::clicked, this, [this, mode] { set_mode(mode); });
    }

    // Conxiones Selector <-> Mapa (canvas)
    connect(&asset_selector, &AssetSelector::clickedImage, &map_canvas, &MapCanvas::set_selected_asset);
}


QHash<uint8_t, AssetData> Editor::populate_hash(const std::string& category_name, const ImageType type) {
    const auto data = toml::parse(CONFIG_PATH "/assets_info.toml");
    auto assets = toml::find<std::vector<AssetData>>(data, category_name);

    QHash<uint8_t, AssetData> hash;
    for (auto& tile: assets) {
        auto path = QString("%1/%2/%3.png").arg(DATA_PATH).arg(category_name.data()).arg(tile.id);
        tile.type = type;

        QSize tile_size(tile.tile_width * TILE_SIZE, tile.tile_height * TILE_SIZE);
        tile.img = QPixmap(path).copy(QRect(QPoint(0, 0), tile_size));

        hash.insert({{tile.id, tile}});
    }

    return hash;
}


void Editor::set_mode(const EditorMode& new_mode) {
    for (const auto& [mode, btn]: action_buttons.asKeyValueRange()) {
        btn->setChecked(mode == new_mode);
    }
    map_canvas.set_mode(new_mode);
}

void Editor::prompt_file_saving() {
    const QString filename = QFileDialog::getSaveFileName();
    if (filename.isEmpty()) {
        return;
    }

    saver.save(filename);
}

void Editor::prompt_file_opening() {
    // Ventana de confirmación
    const QMessageBox::StandardButton reply = QMessageBox::warning(
            this, "Cargar Mapa", "Al cargar un archivo nuevo se borrará el mapa actual. ¿Deseas continuar?",
            QMessageBox::Yes | QMessageBox::No);
    if (reply != QMessageBox::Yes) {
        return;
    }

    // Ventana de buscador de archivos
    const QString filename =
            QFileDialog::getOpenFileName(this, tr("Cargar mapa"), "", tr("bin files (*.bin)"));
    if (filename.isEmpty()) {
        return;
    }

    // Reseteo el editor a nivel lógico y visual
    map_data.clear_all();
    map_canvas.clear_all();

    set_mode(EditorMode::DRAW);
    const bool loaded = loader.load(filename);
    set_mode(EditorMode::DRAG);

    if (!loaded) {
        QMessageBox::critical(this, "Cargar Mapa", "Hubo un error al cargar el mapa.", QMessageBox::Ok);
    }
}

Editor::~Editor() { delete ui; }
