#include "preview_widget.h"

#include "ui_preview_widget.h"

PreviewWidget::PreviewWidget(QHash<QString, QHash<int, QString>>& skins_paths, QWidget* parent):
    QWidget(parent), ui(new Ui::PreviewWidget), skins_paths(skins_paths) {
    ui->setupUi(this);


}

void PreviewWidget::change_body(const int new_id) const {
    const QString path = skins_paths["body"][new_id];
    ui->body->setPixmap(path);
}

void PreviewWidget::change_head(const int new_id) const {
    const QString path = skins_paths["head"][new_id];
    ui->head->setPixmap(path);
}


PreviewWidget::~PreviewWidget() { delete ui; }
