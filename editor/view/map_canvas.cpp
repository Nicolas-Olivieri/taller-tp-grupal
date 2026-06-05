#include "map_canvas.h"

#include <QGraphicsPixmapItem>
#include <QMouseEvent>
#include <QPainter>
#include <QtMath>

#include "ui_mapcanvas.h"

#define TILE_SIZE 32

MapCanvas::MapCanvas(MapData& map_data, QGraphicsView* parent):
        QGraphicsView(parent),
        ui(new Ui::MapCanvas),
        scene(new QGraphicsScene(this)),
        map_data(map_data),
        mode(EditorMode::DRAG) {
    ui->setupUi(this);

    // Setea escena
    this->setScene(scene);
    scene->setSceneRect(0, 0, width(), height());
    this->setRenderHint(QPainter::SmoothPixmapTransform);
    this->setDragMode(ScrollHandDrag);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
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

void MapCanvas::drawBackground(QPainter* painter, const QRectF& rect) {
    QPixmap gridTile(TILE_SIZE, TILE_SIZE);
    gridTile.fill(Qt::white);

    QPainter tilePainter(&gridTile);
    tilePainter.setPen(QColor(220, 220, 220));
    tilePainter.drawLine(0, 0, TILE_SIZE, 0);  // Borde superior
    tilePainter.drawLine(0, 0, 0, TILE_SIZE);  // Borde izquierdo
    tilePainter.end();

    const auto grid_brush = QBrush(gridTile);
    painter->fillRect(rect, grid_brush);
}

void MapCanvas::mouseMoveEvent(QMouseEvent* event) {
    if (mode == EditorMode::DRAG) {
        const QRectF visible_area = this->mapToScene(this->viewport()->rect()).boundingRect();
        const QRectF limits = scene->sceneRect();

        const QRectF safe_zone = limits.adjusted(100, 100, -100, -100);

        if (!safe_zone.contains(visible_area)) {
            QRectF nuevos_limites = limits.united(visible_area);

            nuevos_limites.adjust(-2000, -2000, 2000, 2000);

            scene->setSceneRect(nuevos_limites);
        }

        QGraphicsView::mouseMoveEvent(event);

    } else if (mode == EditorMode::DRAW) {
        asset_preview->show();
        const QPointF scene_pos = mapToScene(event->pos());
        asset_preview->setPos(coordinates_to_grid(scene_pos) * TILE_SIZE);
    }
}

void MapCanvas::mousePressEvent(QMouseEvent* event) {
    if (mode == EditorMode::DRAG) {
        QGraphicsView::mousePressEvent(event);
        return;
    }

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
    if (mode == EditorMode::DRAG) {
        this->setDragMode(ScrollHandDrag);
    }
}

void MapCanvas::set_selected_asset(const AssetData& data) {
    if (mode != EditorMode::DRAW) {
        return;
    }

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
    const int asset_id = map_data.add_asset(clicked_cell, drawing_asset);
    if (asset_id == -1) {
        return;
    }

    add_asset_to_scene(clicked_cell, asset_id);
}

void MapCanvas::add_asset_to_scene(const QPoint clicked_cell, const int asset_id) {
    set_unwalkable_tiles(clicked_cell, asset_id);

    const auto tile = new QGraphicsPixmapItem(asset_preview->pixmap());
    tile->setPos(clicked_cell * TILE_SIZE);
    tile->setData(0, asset_id);
    scene->addItem(tile);
}

void MapCanvas::erase_asset(const QPointF clicked_pos) const {
    const QList<QGraphicsItem*> cell_assets = scene->items(clicked_pos);
    if (cell_assets.empty() || !cell_assets.first()->data(0).isValid() ||
        cell_assets.first()->group() == unwalkable_tiles) {
        return;
    }

    QGraphicsItem* clicked_asset = cell_assets.first();
    const bool erased = map_data.erase_asset(clicked_asset->data(0).toInt());
    if (!erased) {
        return;
    }

    erase_unwalkable_tiles(clicked_asset->data(0).toInt());

    scene->removeItem(clicked_asset);
    delete clicked_asset;
}

void MapCanvas::clear_all() {
    QList<QGraphicsItem*> assets = scene->items();
    for (const auto asset: assets) {
        // Evito eliminar los elementos necesarios para el funcionamiento del editor
        if (asset != asset_preview && asset != unwalkable_tiles) {
            scene->removeItem(asset);
        }
    }
}


void MapCanvas::set_unwalkable_tiles(const QPoint& clicked_cell, const int tile_id) const {
    const QPoint unwalkable_offset = drawing_asset.unwalkable_area.topLeft();
    const QSize unwalkable_size = drawing_asset.unwalkable_area.size();

    const QBrush redBrush(QColor(255, 0, 0, 150));
    const QPen noPen(Qt::NoPen);
    for (int i = unwalkable_offset.x(); i < unwalkable_size.width()+unwalkable_offset.x(); i++) {
        for (int j = unwalkable_offset.y(); j < unwalkable_size.height()+unwalkable_offset.y(); j++) {
            auto* mark = new QGraphicsEllipseItem(11, 11, 10, 10);
            mark->setZValue(98.0);
            mark->setData(0, tile_id);
            mark->setBrush(redBrush);
            mark->setPen(noPen);
            mark->setPos((clicked_cell.x() + i) * TILE_SIZE, (clicked_cell.y() + j) * TILE_SIZE);

            unwalkable_tiles->addToGroup(mark);
        }
    }
}

void MapCanvas::erase_unwalkable_tiles(const int tile_id) const {
    QList<QGraphicsItem*> marks = unwalkable_tiles->childItems();

    for (const auto& mark: marks) {
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

MapCanvas::~MapCanvas() {
    for (const auto item: scene->items()) {
        delete item;
    }

    delete ui;
}
