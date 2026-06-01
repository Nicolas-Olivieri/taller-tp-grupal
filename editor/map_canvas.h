#ifndef MAP_CANVAS_H
#define MAP_CANVAS_H

#include <QGraphicsView>

#include "components.h"
#include "map_data.h"
#include "client/client.h"

namespace Ui {
class MapCanvas;
}

class MapCanvas: public QGraphicsView {
friend class MapLoader;

    Q_OBJECT
public:
    explicit MapCanvas(MapData& map_data, QGraphicsView* parent = nullptr);

    void set_mode(EditorMode new_mode);

    ~MapCanvas();

private:
    void place_asset(QPointF clicked_pos);
    void add_asset_to_scene(QPoint clicked_cell, int asset_id);

    void erase_asset(QPointF clicked_pos);

void erase_all_assets();

void set_unwalkable_tiles(const QPoint &clicked_cell, int tile_id) const;
    void erase_unwalkable_tiles(int tile_id) const;

    QPoint coordinates_to_grid(QPointF coordinates) const;


public slots:
    void set_selected_asset(const AssetData& data);
    void set_visibility_unwalkables() const;

protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;


private:
    Ui::MapCanvas* ui;
    QGraphicsScene* scene;

    MapData& map_data;

    EditorMode mode;
    AssetData drawing_asset;
    QGraphicsPixmapItem* asset_preview;
    QGraphicsItemGroup* unwalkable_tiles;
};

#endif  // MAP_CANVAS_H
