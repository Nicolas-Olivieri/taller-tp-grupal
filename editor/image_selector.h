#ifndef IMAGE_SELECTOR_H
#define IMAGE_SELECTOR_H

#include <QTabWidget>

namespace Ui {
class ImageSelector;
}

class ImageSelector: public QTabWidget {
    Q_OBJECT

public:
    explicit ImageSelector(QWidget* parent = nullptr);
    ~ImageSelector();

private:
    Ui::ImageSelector* ui;
};

#endif  // IMAGE_SELECTOR_H
