#include "map_canvas.h"

#include "ui_mapcanvas.h"
#include <QPainter>
#include <QMouseEvent>
#include <QGraphicsPixmapItem>
#include <QtMath>

#define TILE_SIZE 32

MapCanvas::MapCanvas(/*const QHash<uint8_t, AssetData>& tiles,
                     const QHash<uint8_t, AssetData>& colliders, */ QGraphicsView* parent):
    QGraphicsView(parent), ui(new Ui::MapCanvas), scene(new QGraphicsScene(this)),
    map(EditorMap()), /*tiles(tiles), colliders(colliders),*/ mode(EditorMode::SELECT)
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

    // Setea capa de no caminable
    unwalkable_tiles = scene->createItemGroup({});
    unwalkable_tiles->setZValue(98.0);
    unwalkable_tiles->setVisible(false);
}


// MÉTODOS DE QGRAPHICSVIEW ::::::::::::::::::::::::::::

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
    if (mode != EditorMode::DRAW) {return;}

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


// MÉTODOS DE SETEO ::::::::::::::::::::::::::::

void MapCanvas::set_mode(const EditorMode new_mode) {
    mode = new_mode;

    if (mode != EditorMode::DRAW) {
        asset_preview->hide();
    }
}

void MapCanvas::set_selected_asset(const AssetData& data) {
    if (mode != EditorMode::DRAW) {return;}

    drawing_asset = data;
    asset_preview->hide();
    asset_preview->setPixmap(data.img);
}

void MapCanvas::set_visibility_unwalkables() const {
    const bool curr_state = unwalkable_tiles->isVisible();
    unwalkable_tiles->setVisible(!curr_state);
    QGraphicsItemGroup group(unwalkable_tiles);
}


// MÉTODOS DE ASSETS ::::::::::::::::::::::::::::

void MapCanvas::place_asset(const QPointF clicked_pos) {
    const QPoint clicked_cell = coordinates_to_grid(clicked_pos);
    const int asset_id = map.add_asset(clicked_cell, drawing_asset);
    if (asset_id == -1) {return;}

    set_unwalkable_tiles(clicked_cell, asset_id);

    const auto tile = new QGraphicsPixmapItem(asset_preview->pixmap());
    tile->setPos(asset_preview->pos());
    tile->setData(0, asset_id);
    scene->addItem(tile);
}

void MapCanvas::erase_asset(const QPointF clicked_pos) {
    const QList<QGraphicsItem*> cell_assets = scene->items(clicked_pos);
    if (cell_assets.empty() ||
        !cell_assets.first()->data(0).isValid() ||
        cell_assets.first()->group() == unwalkable_tiles) {return;}

    QGraphicsItem* clicked_asset = cell_assets.first();
    const bool erased = map.erase_asset(clicked_asset->data(0).toInt());
    if (!erased) { return; }

    erase_unwalkable_tiles(clicked_asset->data(0).toInt());

    scene->removeItem(clicked_asset);
    delete clicked_asset;
}

void MapCanvas::set_unwalkable_tiles(const QPoint& clicked_cell, const int tile_id) const {
    const QPoint unwalkable_offset = drawing_asset.unwalkable_tiles.topLeft();
    const QSize unwalkable_size = drawing_asset.unwalkable_tiles.size();

    const QBrush redBrush(QColor(255, 0, 0, 150));
    const QPen noPen(Qt::NoPen);
    for (int i = unwalkable_offset.x(); i < unwalkable_size.width(); i++) {
        for (int j = unwalkable_offset.y(); j < unwalkable_size.height(); j++) {
            auto* mark = new QGraphicsEllipseItem(11,11,10,10);
            mark->setZValue(98.0);
            mark->setData(0, tile_id);
            mark->setBrush(redBrush);
            mark->setPen(noPen);
            mark->setPos((clicked_cell.x()+i)*TILE_SIZE, (clicked_cell.y()+j)*TILE_SIZE);

            unwalkable_tiles->addToGroup(mark);
        }
    }
}

void MapCanvas::erase_unwalkable_tiles(const int tile_id) const {
    QList<QGraphicsItem*> marks = unwalkable_tiles->childItems();
    for (const auto & mark : marks) {
        if (mark->data(0) == tile_id) {
            unwalkable_tiles->removeFromGroup(mark);
            delete mark;
        }
    }
}


QPoint MapCanvas::coordinates_to_grid(const QPointF coordinates) const {
    const int x_grid = qFloor(coordinates.x() / TILE_SIZE);
    const int y_grid = qFloor(coordinates.y() / TILE_SIZE);

    return {x_grid, y_grid};
}

MapCanvas::~MapCanvas() { delete ui; }
