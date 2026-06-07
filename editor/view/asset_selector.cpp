#include "asset_selector.h"

#include <qlistwidget.h>
#include <sys/socket.h>

#include "ui_assetselector.h"

AssetSelector::AssetSelector(const QHash<uint8_t, AssetData>& tiles,
                             const QHash<uint8_t, AssetData>& colliders,
                             const QHash<uint8_t, AssetData>& npcs, QWidget* parent):
        QTabWidget(parent), ui(new Ui::AssetSelector), tiles(tiles), colliders(colliders), npcs(npcs) {

    ui->setupUi(this);

    for (const auto& tile_data: tiles) {
        auto* item = new QListWidgetItem(QIcon(tile_data.img), tile_data.name);
        item->setData(Qt::UserRole, tile_data.id);
        ui->tiles_list->addItem(item);
    }

    for (const auto& coll_data: colliders) {
        auto* item = new QListWidgetItem(QIcon(coll_data.img), coll_data.name);
        item->setData(Qt::UserRole, coll_data.id);
        ui->colliders_list->addItem(item);
    }

    for (const auto& npcs_data: npcs) {
        auto* item = new QListWidgetItem(npcs_data.img, npcs_data.name);
        item->setData(Qt::UserRole, npcs_data.id);
        ui->npcs_list->addItem(item);
    }

    connect(ui->tiles_list, &QListWidget::itemClicked, this, [this, tiles](const QListWidgetItem* item) {
        int id = item->data(Qt::UserRole).toInt();
        emit clickedImage(tiles.value(id));
    });

    connect(ui->colliders_list, &QListWidget::itemClicked, this,
            [this, colliders](const QListWidgetItem* item) {
                int id = item->data(Qt::UserRole).toInt();
                emit clickedImage(colliders.value(id));
            });

    connect(ui->npcs_list, &QListWidget::itemClicked, this, [this, npcs](const QListWidgetItem* item) {
        int id = item->data(Qt::UserRole).toInt();
        emit clickedImage(npcs.value(id));
    });
}


AssetSelector::~AssetSelector() { delete ui; }
