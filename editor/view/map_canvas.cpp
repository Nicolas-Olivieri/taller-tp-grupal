#include "map_canvas.h"

#include <QGraphicsPixmapItem>
#include <QMouseEvent>
#include <QPainter>
#include <QtMath>
#include <iostream>

#include "editor_constants.h"
#include "grid_range.h"
#include "ui_mapcanvas.h"


MapCanvas::MapCanvas(MapData& map_data, QWidget* parent):
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

    auto& config = EditorConfig::get();
    const auto tile_size = config.get_tile_size();
    const auto safe_zone_data = config.get_safe_zone_data();

    // Setea preview de zona segura
    safe_preview = new QGraphicsRectItem(0, 0, safe_zone_data.brush_w * tile_size,
                                         safe_zone_data.brush_h * tile_size);
    safe_preview->setBrush(QBrush(QColor(95, 170, 50)));
    safe_preview->setPen(Qt::NoPen);
    safe_preview->setOpacity(0.5);
    safe_preview->setZValue(99.0);
    safe_preview->hide();
    scene->addItem(safe_preview);

    // Setea capa de no caminable
    unwalkable_tiles = scene->createItemGroup({});
    unwalkable_tiles->setZValue(98.0);
    unwalkable_tiles->setVisible(false);

    // Setea capa de zona segura
    safe_tiles = scene->createItemGroup({});
    safe_tiles->setZValue(99.0);
    safe_tiles->setVisible(false);
}


// MÉTODOS DE QGRAPHICSVIEW ::::::::::::::::::::::::::::

void MapCanvas::drawBackground(QPainter* painter, const QRectF& rect) {
    const uint16_t tile_size = EditorConfig::get().get_tile_size();

    QPixmap gridTile(tile_size, tile_size);
    gridTile.fill(Qt::white);

    QPainter tilePainter(&gridTile);
    tilePainter.setPen(QColor(220, 220, 220));
    tilePainter.drawLine(0, 0, tile_size, 0);  // Borde superior
    tilePainter.drawLine(0, 0, 0, tile_size);  // Borde izquierdo
    tilePainter.end();

    const auto grid_brush = QBrush(gridTile);
    painter->fillRect(rect, grid_brush);
}

void MapCanvas::mouseMoveEvent(QMouseEvent* event) {
    const uint16_t tile_size = EditorConfig::get().get_tile_size();

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
        asset_preview->setPos(coordinates_to_grid(scene_pos) * tile_size);

    } else if (mode == EditorMode::SAFE_ZONE) {
        safe_preview->show();
        const QPointF scene_pos = mapToScene(event->pos());
        safe_preview->setPos(coordinates_to_grid(scene_pos) * tile_size);
    }
}

void MapCanvas::mousePressEvent(QMouseEvent* event) {
    const QPointF scene_pos = mapToScene(event->pos());
    switch (mode) {
        case EditorMode::DRAG:
            QGraphicsView::mousePressEvent(event);
            return;
        case EditorMode::DRAW:
            place_asset(scene_pos);
            break;
        case EditorMode::ERASE:
            erase_asset(scene_pos);
            break;
        case EditorMode::SAFE_ZONE:
            set_safe_tiles(scene_pos);
            break;
    }
}


// MÉTODOS DE SETEO ::::::::::::::::::::::::::::

void MapCanvas::set_mode(const EditorMode new_mode) {
    mode = new_mode;

    if (mode != EditorMode::DRAW) {
        asset_preview->hide();
    }
    if (mode != EditorMode::SAFE_ZONE) {
        safe_preview->hide();
    }
    if (mode == EditorMode::DRAG) {
        this->setDragMode(ScrollHandDrag);
    }
    if (mode == EditorMode::SAFE_ZONE) {}
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

void MapCanvas::set_visibility_safes() const {
    const bool curr_state = safe_tiles->isVisible();
    safe_tiles->setVisible(!curr_state);
    QGraphicsItemGroup group(safe_tiles);
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

    tile->setPos(clicked_cell * EditorConfig::get().get_tile_size());
    tile->setData(0, asset_id);
    scene->addItem(tile);
}

void MapCanvas::erase_asset(const QPointF clicked_pos) const {
    // Valido que no sea un punto de colisión
    const QList<QGraphicsItem*> cell_assets = scene->items(clicked_pos);
    if (cell_assets.empty() || !cell_assets.first()->data(0).isValid() ||
        cell_assets.first()->group() == unwalkable_tiles) {
        return;
    }

    // En caso de ser visible la zona segura, se borra ese componente, sino se borra el collider o tile
    QGraphicsItem* clicked_asset = cell_assets.first();
    if (clicked_asset->group() == safe_tiles && safe_tiles->isVisible()) {
        map_data.erase_safe_tile(coordinates_to_grid(clicked_pos));

    } else {
        const bool erased = map_data.erase_asset(clicked_asset->data(0).toInt());
        if (!erased) {
            return;
        }
        erase_unwalkable_tiles(clicked_asset->data(0).toInt());
    }

    scene->removeItem(clicked_asset);
    delete clicked_asset;
}

void MapCanvas::clear_all() {
    QList<QGraphicsItem*> assets = scene->items();
    for (const auto asset: assets) {
        // Evito eliminar los elementos necesarios para el funcionamiento del editor
        if (asset != asset_preview && asset != unwalkable_tiles && asset != safe_preview &&
            asset != safe_tiles) {
            scene->removeItem(asset);
        }
    }
}


void MapCanvas::set_unwalkable_tiles(const QPoint& clicked_cell, const int tile_id) const {
    const GridRange grid_range(clicked_cell, drawing_asset.tile_width, drawing_asset.tile_height);
    const QRect unwalkable_area = drawing_asset.unwalkable_area.translated(clicked_cell);

    const QBrush redBrush(QColor(255, 0, 0, 150));
    const QPen noPen(Qt::NoPen);
    for (const auto& cell: grid_range) {

        if ((!drawing_asset.inverse_unwalkable && unwalkable_area.contains(cell)) ||
            (drawing_asset.inverse_unwalkable && !unwalkable_area.contains(cell))) {
            auto* mark = new QGraphicsEllipseItem(11, 11, 10, 10);
            mark->setZValue(98.0);
            mark->setData(0, tile_id);
            mark->setBrush(redBrush);
            mark->setPen(noPen);
            mark->setPos(cell * EditorConfig::get().get_tile_size());

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

void MapCanvas::set_safe_tiles(const QPointF& clicked_pos, const int width, const int height) const {
    const uint16_t tile_size = EditorConfig::get().get_tile_size();

    const QPoint clicked_cell = coordinates_to_grid(clicked_pos);
    QSet<QPair<int, QPoint>> added_tiles = map_data.add_safe_tiles(clicked_cell, width, height);

    const QBrush greenBrush(QColor(95, 170, 50, 100));
    const QPen noPen(Qt::NoPen);
    for (const auto& [id, cell]: added_tiles) {
        auto* mark = new QGraphicsRectItem(0, 0, tile_size, tile_size);
        mark->setZValue(98.0);
        mark->setData(0, id);
        mark->setBrush(greenBrush);
        mark->setPen(noPen);
        mark->setPos(cell * tile_size);

        safe_tiles->addToGroup(mark);
    }
}


QPoint MapCanvas::coordinates_to_grid(const QPointF coordinates) const {
    const uint16_t tile_size = EditorConfig::get().get_tile_size();

    const int x_grid = qFloor(coordinates.x() / tile_size);
    const int y_grid = qFloor(coordinates.y() / tile_size);

    return {x_grid, y_grid};
}

MapCanvas::~MapCanvas() {
    for (const auto item: scene->items()) {
        delete item;
    }

    delete ui;
}
