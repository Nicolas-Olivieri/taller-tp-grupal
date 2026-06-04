#ifndef CREATE_WINDOW_H
#define CREATE_WINDOW_H

#include <QButtonGroup>
#include <QLabel>
#include <QWidget>

#include "preview_widget.h"
#include "selector_widget.h"

namespace Ui {
class CreateWindow;
}

class CreateWindow: public QWidget {
    Q_OBJECT

public:
    explicit CreateWindow(QWidget* parent = nullptr);

    ~CreateWindow();

private:
    Ui::CreateWindow* ui;
    SelectorWidget* head_selector;
    SelectorWidget* body_selector;
    PreviewWidget* preview;

    QHash<QString, QHash<int, QString>> skins_paths;
    QHash<QString, QVector<int>> skin_ranges;

    void load_skins_data();
};

#endif  // CREATE_WINDOW_H
