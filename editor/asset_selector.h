#ifndef IMAGE_SELECTOR_H
#define IMAGE_SELECTOR_H

#include <QTabWidget>
#include <vector>

#include "components.h"

namespace Ui {
class AssetSelector;
}

class AssetSelector: public QTabWidget {
    Q_OBJECT

public:
    AssetSelector(const QHash<uint8_t, AssetData> &tiles, const QHash<uint16_t, AssetData> &colliders, QWidget *parent = nullptr);

    ~AssetSelector();

signals:
    void clickedImage(const AssetData &data);


private:
    Ui::AssetSelector* ui{};

    const QHash<uint8_t, AssetData>& tiles;
    const QHash<uint16_t, AssetData>& colliders;
};

#endif  // IMAGE_SELECTOR_H
