#ifndef IMAGE_SELECTOR_H
#define IMAGE_SELECTOR_H

#include <QTabWidget>
#include <vector>

#include "components.h"

namespace Ui {
class ImageSelector;
}

class ImageSelector: public QTabWidget {
    Q_OBJECT

public:
    explicit ImageSelector(QWidget* parent = nullptr);

    // TODO parece que no se usa, si es el caso, eliminarlo
    ImageSelector(const std::vector<TileView>& tiles, const std::vector<CollidableView>& colliders,
                  QWidget* parent = nullptr);

    ~ImageSelector();

    void add_elements(const std::vector<TileView>& tiles, const std::vector<CollidableView>& colliders);

private:
    Ui::ImageSelector* ui;
};

#endif  // IMAGE_SELECTOR_H
