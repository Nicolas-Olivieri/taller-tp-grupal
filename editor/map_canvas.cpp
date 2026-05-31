#include "map_canvas.h"

#include "ui_mapcanvas.h"
#include <QPainter>
#include <QMouseEvent>
#include <QGraphicsPixmapItem>
#include <QtMath>

#define TILE_SIZE 32

MapCanvas::MapCanvas(const QHash<uint8_t, AssetData>& tiles,
                     const QHash<uint8_t, AssetData>& colliders, QGraphicsView* parent):
    QGraphicsView(parent), ui(new Ui::MapCanvas), scene(new QGraphicsScene(this)),
    map(EditorMap()), tiles(tiles), colliders(colliders), mode(EditorMode::NONE)
{
    ui->setupUi(this);

    // Setea escena
    this->setScene(scene);
    this->setRenderHint(QPainter::SmoothPixmapTransform);
    this->setDragMode(ScrollHandDrag);
    this->setMouseTracking(true);

    // Setea preview de imagen
    asset_preview = new QGraphicsPixmapItem();
    asset_preview->setOpacity(0.5);
    asset_preview->setZValue(99.0);
    scene->addItem(asset_preview);
}

void MapCanvas::setMode(const EditorMode new_mode) {
    mode = new_mode;

    if (mode == EditorMode::ERASE) {
        asset_preview->hide();
    }
}


void MapCanvas::setSelectedAsset(const AssetData& data) {
    selected_asset = data;
    mode = EditorMode::DRAW;

    asset_preview->hide();
    asset_preview->setPixmap(data.img);
}


void MapCanvas::drawBackground(QPainter *painter, const QRectF &rect) {
    QPixmap gridTile(TILE_SIZE, TILE_SIZE);
    gridTile.fill(Qt::white);

    QPainter tilePainter(&gridTile);
    tilePainter.setPen(QColor(220, 220, 220));
    tilePainter.drawLine(0, 0, TILE_SIZE, 0); // Borde superior
    tilePainter.drawLine(0, 0, 0, TILE_SIZE); // Borde izquierdo
    tilePainter.end();

    const auto grid_brush = QBrush(gridTile);
    painter->fillRect(rect, grid_brush);
}

void MapCanvas::mouseMoveEvent(QMouseEvent *event) {
    if (mode == EditorMode::ERASE) {return;}

    asset_preview->show();
    const QPointF scene_pos = mapToScene(event->pos());
    asset_preview->setPos(coordinates_to_grid(scene_pos)*TILE_SIZE);
}

void MapCanvas::mousePressEvent(QMouseEvent *event) {
    const QPointF scene_pos = mapToScene(event->pos());

    if (mode == EditorMode::DRAW) {
        place_asset(scene_pos);
    } else if (mode == EditorMode::ERASE) {
        erase_asset(scene_pos);
    }
}

void MapCanvas::place_asset(const QPointF clicked_pos) {
    const QPoint clicked_cell = coordinates_to_grid(clicked_pos);
    const int asset_id = map.add_asset(clicked_cell, selected_asset);
    if (asset_id == -1) {return;}

    const auto tile = new QGraphicsPixmapItem(asset_preview->pixmap());
    tile->setPos(asset_preview->pos());
    tile->setData(0, asset_id);
    scene->addItem(tile);
}

void MapCanvas::erase_asset(const QPointF clicked_pos) {
    const QList<QGraphicsItem*> cell_assets = scene->items(clicked_pos);

    if (cell_assets.empty() || !cell_assets.first()->data(0).isValid()) {return;}

    QGraphicsItem* clicked_asset = cell_assets.first();
    const bool erased = map.erase_asset(clicked_asset->data(0).toInt());
    if (!erased) { return; }

    scene->removeItem(clicked_asset);
    delete clicked_asset;
}


QPoint MapCanvas::coordinates_to_grid(const QPointF coordinates) const {
    const int x_grid = qFloor(coordinates.x() / TILE_SIZE);
    const int y_grid = qFloor(coordinates.y() / TILE_SIZE);

    return {x_grid, y_grid};
}

MapCanvas::~MapCanvas() { delete ui; }
