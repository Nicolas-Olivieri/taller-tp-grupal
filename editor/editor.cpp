#include "editor.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QShortcut>
#include <string>
#include <vector>

#include "config/editor_config.h"
#include "view/tutorial_overlay.h"

#include "editor_constants.h"
#include "ui_editor.h"


Editor::Editor(QWidget* parent):
        QMainWindow(parent),
        ui(new Ui::Editor),
        tiles(populate_hash("tiles", ImageType::TILE)),
        colliders(populate_hash("colliders", ImageType::COLLIDER)),
        npcs(populate_hash("npcs", ImageType::NPC)),
        tutorial(TutorialOverlay(this)),
        map_data(MapData()),
        map_canvas(MapCanvas(this->map_data, this)),
        asset_selector(this->tiles, this->colliders, this->npcs, this),
        loader(this->map_data, this->map_canvas, tiles, colliders, npcs),
        saver(MapSaver(this->map_data)) {

    ui->setupUi(this);
    ui->mapWidget->addWidget(&map_canvas);
    ui->selectorWidget->addWidget(&asset_selector);

    const auto& shortcuts = EditorConfig::get().get_shortcuts_keys();
    // Conexión botones
    const auto* draw_sc = new QShortcut(QKeySequence(shortcuts.draw_key.c_str()), this);
    connect(ui->drawBtn, &QPushButton::clicked, this, [this] { set_mode(EditorMode::DRAW); });
    connect(draw_sc, &QShortcut::activated, this, [this] { set_mode(EditorMode::DRAW); });

    const auto* drag_sc = new QShortcut(QKeySequence(shortcuts.drag_key.c_str()), this);
    connect(ui->dragBtn, &QPushButton::clicked, this, [this] { set_mode(EditorMode::DRAG); });
    connect(drag_sc, &QShortcut::activated, this, [this] { set_mode(EditorMode::DRAG); });

    const auto* erase_sc = new QShortcut(QKeySequence(shortcuts.erase_key.c_str()), this);
    connect(ui->eraseBtn, &QPushButton::clicked, this, [this] { set_mode(EditorMode::ERASE); });
    connect(erase_sc, &QShortcut::activated, this, [this] { set_mode(EditorMode::ERASE); });

    const auto* safe_sc = new QShortcut(QKeySequence(shortcuts.safe_zone_key.c_str()), this);
    connect(ui->safeZoneBtn, &QPushButton::clicked, this, [this] {
        if (!ui->cbox_safes->isChecked()) {
            ui->cbox_safes->click();
        }
        set_mode(EditorMode::SAFE_ZONE);
    });
    connect(safe_sc, &QShortcut::activated, this, [this] {
        if (!ui->cbox_safes->isChecked()) {
            ui->cbox_safes->click();
        }
        set_mode(EditorMode::SAFE_ZONE);
    });

    config_tutorial();

    connect(ui->saveBtn, &QPushButton::clicked, this, &Editor::prompt_file_saving);
    connect(ui->loadBtn, &QPushButton::clicked, this, &Editor::prompt_file_opening);
    connect(ui->helpBtn, &QPushButton::clicked, &tutorial, &TutorialOverlay::start);
    connect(ui->cbox_unwalkables, &QCheckBox::clicked, &map_canvas, &MapCanvas::set_visibility_unwalkables);
    connect(ui->cbox_safes, &QCheckBox::clicked, &map_canvas, &MapCanvas::set_visibility_safes);
    connect(&asset_selector, &AssetSelector::clickedImage, &map_canvas, &MapCanvas::set_selected_asset);
}


QHash<uint8_t, AssetData> Editor::populate_hash(const std::string& category_name, const ImageType type) {
    auto assets = EditorConfig::get().get_assets_data(category_name);

    const auto standard_tile_size = EditorConfig::get().get_tile_size();

    QHash<uint8_t, AssetData> hash;
    for (auto& tile: assets) {
        auto path = QString("%1/%2/%3.png").arg(DATA_PATH).arg(category_name.data()).arg(tile.id);
        tile.type = type;

        QSize tile_size(tile.tile_width * standard_tile_size, tile.tile_height * standard_tile_size);
        tile.img = QPixmap(path).copy(QRect(QPoint(0, 0), tile_size));

        hash.insert({{tile.id, tile}});
    }

    return hash;
}

void Editor::config_tutorial() {
    const auto& tutorial_info = EditorConfig::get().get_tutorial_data();

    tutorial.add_step(ui->dragBtn, QString::fromStdString(tutorial_info.drag_btn));
    tutorial.add_step(ui->drawBtn, QString::fromStdString(tutorial_info.draw_btn));
    tutorial.add_step(ui->eraseBtn, QString::fromStdString(tutorial_info.erase_btn));
    tutorial.add_step(ui->safeZoneBtn, QString::fromStdString(tutorial_info.safe_zone_btn));
    tutorial.add_step(ui->cbox_unwalkables, QString::fromStdString(tutorial_info.unwalkable_cbox));
    tutorial.add_step(ui->cbox_safes, QString::fromStdString(tutorial_info.safe_zone_cbox));
    tutorial.add_step(asset_selector.childAt(0, 0), QString::fromStdString(tutorial_info.asset_info));
    tutorial.add_step(ui->loadBtn, QString::fromStdString(tutorial_info.load_btn));
    tutorial.add_step(ui->saveBtn, QString::fromStdString(tutorial_info.save_btn));
}


void Editor::set_mode(const EditorMode& new_mode) { map_canvas.set_mode(new_mode); }

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
