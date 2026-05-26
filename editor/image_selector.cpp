#include "image_selector.h"

#include "ui_imageselector.h"

ImageSelector::ImageSelector(QWidget* parent): QTabWidget(parent), ui(new Ui::ImageSelector) {
    ui->setupUi(this);
}

ImageSelector::~ImageSelector() { delete ui; }
