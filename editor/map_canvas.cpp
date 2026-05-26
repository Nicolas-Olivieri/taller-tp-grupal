#include "map_canvas.h"

#include "ui_mapcanvas.h"

MapCanvas::MapCanvas(QWidget* parent): QWidget(parent), ui(new Ui::MapCanvas) { ui->setupUi(this); }

MapCanvas::~MapCanvas() { delete ui; }
