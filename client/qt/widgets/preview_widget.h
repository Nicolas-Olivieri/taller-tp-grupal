#ifndef PREVIEW_WIDGET_H
#define PREVIEW_WIDGET_H

#include <QWidget>

namespace Ui {
class PreviewWidget;
}

class PreviewWidget: public QWidget {
    Q_OBJECT

public:
    explicit PreviewWidget(QHash<QString, QHash<int, QString>>& skins_paths, QWidget* parent = nullptr);

    ~PreviewWidget();

public slots:
    void change_body(int new_id) const;

    void change_head(int new_id) const;

private:
    Ui::PreviewWidget* ui;
    QHash<QString, QHash<int, QString>>& skins_paths;
};

#endif  // PREVIEW_WIDGET_H
