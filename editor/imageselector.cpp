#include "imageselector.h"

#include "ui_imageselector.h"

ImageSelector::ImageSelector(const std::vector<TileView>& tiles, const std::vector<CollidableView>& colliders,
                             QWidget* parent):
        QTabWidget(parent), ui(new Ui::ImageSelector) {

    ui->setupUi(this);

    for (const TileView& tile_view: tiles) {
        auto* item = new QListWidgetItem(QIcon(tile_view.img), tile_view.name);
        item->setData(Qt::UserRole, tile_view.id);
        ui->listWidget_2->addItem(item);
    }


    for (const CollidableView& coll_view: colliders) {
        auto* item = new QListWidgetItem(QIcon(coll_view.img), coll_view.name);
        item->setData(Qt::UserRole, coll_view.id);
        ui->listWidget->addItem(item);
    }
}

ImageSelector::~ImageSelector() { delete ui; }
ImageSelector::ImageSelector(QWidget* parent): QTabWidget(parent), ui(new Ui::ImageSelector) {
    ui->setupUi(this);
    // importante que el setup va en el constructor y siempre antes de que se use algo de ui
}

void ImageSelector::add_elements(const std::vector<TileView>& tiles,
                                 const std::vector<CollidableView>& colliders) {
    for (const TileView& tile_view: tiles) {
        auto* item = new QListWidgetItem(QIcon(tile_view.img), tile_view.name);
        item->setData(Qt::UserRole, tile_view.id);
        ui->listWidget_2->addItem(item);
    }


    for (const CollidableView& coll_view: colliders) {
        auto* item = new QListWidgetItem(QIcon(coll_view.img), coll_view.name);
        item->setData(Qt::UserRole, coll_view.id);
        ui->listWidget->addItem(item);
    }
}
