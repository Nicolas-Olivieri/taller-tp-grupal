#ifndef MAPCANVAS_H
#define MAPCANVAS_H

#include <QWidget>

namespace Ui {
class MapCanvas;
}

class MapCanvas: public QWidget {
    Q_OBJECT

public:
    explicit MapCanvas(QWidget* parent = nullptr);
    ~MapCanvas();

private:
    Ui::MapCanvas* ui;
};

#endif  // MAPCANVAS_H
