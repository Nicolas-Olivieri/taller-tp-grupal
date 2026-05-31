#include "map_canvas.h"

#include "ui_mapcanvas.h"
#include <QPainter>
#include <QMouseEvent>
#include <QGraphicsPixmapItem>

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
    scene->addItem(asset_preview);
}

void MapCanvas::setMode(const EditorMode new_mode) {
    mode = new_mode;
}


void MapCanvas::setSelectedAsset(const AssetData& data) {
    selected_asset = data;
    mode = EditorMode::DRAW;

    asset_preview->hide();
    asset_preview->setPixmap(data.img);
}


void MapCanvas::drawBackground(QPainter *painter, const QRectF &rect) {
    QPixmap gridTile(32, 32);
    gridTile.fill(Qt::white);

    QPainter tilePainter(&gridTile);
    tilePainter.setPen(QColor(220, 220, 220));
    tilePainter.drawLine(0, 0, 32, 0); // Borde superior
    tilePainter.drawLine(0, 0, 0, 32); // Borde izquierdo
    tilePainter.end();

    const auto grid_brush = QBrush(gridTile);
    painter->fillRect(rect, grid_brush);
}

void MapCanvas::mouseMoveEvent(QMouseEvent *event) {
    asset_preview->show();
    const QPointF scene_pos = mapToScene(event->pos());
    asset_preview->setPos(coordinates_to_grid(scene_pos));
}

void MapCanvas::mousePressEvent(QMouseEvent *event) {
    if (mode == EditorMode::DRAW) {
        placeTile();
    }
    std::cout << event->flags() <<std::endl;
}

void MapCanvas::placeTile() {
    const bool placed = map.add_tile(coordinates_to_grid(asset_preview->pos()), selected_asset);
    if (!placed) {return;}

    const auto tile = new QGraphicsPixmapItem(asset_preview->pixmap());
    tile->setPos(asset_preview->pos());
    scene->addItem(tile);
}

QPoint MapCanvas::coordinates_to_grid(const QPointF coordinates) const {
    int x_grid = static_cast<int>(coordinates.x()) / 32;
    int y_grid = static_cast<int>(coordinates.y()) / 32;

    // Si los números son negativos, hay que ajustar el offset
    if (coordinates.x() < 0) x_grid--;
    if (coordinates.y() < 0) y_grid--;

    return QPoint(x_grid * 32, y_grid * 32);
}

MapCanvas::~MapCanvas() { delete ui; }
