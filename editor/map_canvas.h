#ifndef MAP_CANVAS_H
#define MAP_CANVAS_H

#include <QGraphicsView>

#include "components.h"
#include "editor_map.h"
#include "client/client.h"

namespace Ui {
class MapCanvas;
}

class MapCanvas: public QGraphicsView {
    Q_OBJECT

public:
    explicit MapCanvas(const QHash<uint8_t, AssetData>& tiles,
                       const QHash<uint8_t, AssetData>& colliders,
                       QGraphicsView* parent = nullptr);

    void setMode(EditorMode new_mode);

    ~MapCanvas();

private:
    void place_asset(QPointF clicked_pos);
    void erase_asset(QPointF clicked_pos);
    QPoint coordinates_to_grid(QPointF coordinates) const;


public slots:
    void setSelectedAsset(const AssetData& data);


protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;


private:
    Ui::MapCanvas* ui;
    QGraphicsScene* scene;

    EditorMap map;
    const QHash<uint8_t, AssetData>& tiles;
    const QHash<uint8_t, AssetData>& colliders;

    EditorMode mode;

    AssetData selected_asset;
    QGraphicsPixmapItem* asset_preview;
};

#endif  // MAP_CANVAS_H
