#include "resolution_widget.h"

#include <string>

#include "client/client.h"

#include "ui_resolution_widget.h"

ResolutionWidget::ResolutionWidget(QWidget* parent): QWidget(parent), ui(new Ui::ResolutionWidget) {
    ui->setupUi(this);

    resolution_values << "1024x726"
                      << "1280x800"
                      << "1366x768"
                      << "1920x1080";
    this->curr_opt = 0;
    ui->resolution->setText(resolution_values[curr_opt]);

    connect(ui->btnLeft, &QPushButton::clicked, this, &ResolutionWidget::on_left_clicked);
    connect(ui->btnRight, &QPushButton::clicked, this, &ResolutionWidget::on_right_clicked);
}

void ResolutionWidget::on_left_clicked() {
    int val = curr_opt > 0 ? --curr_opt : resolution_values.size() - 1;
    curr_opt = val;
    ui->resolution->setText(resolution_values[curr_opt]);
}

void ResolutionWidget::on_right_clicked() {
    int val = curr_opt < resolution_values.size() - 1 ? ++curr_opt : 0;
    curr_opt = val;
    ui->resolution->setText(resolution_values[curr_opt]);
}

std::string ResolutionWidget::get_resolution() const { return resolution_values[curr_opt].toStdString(); }

ResolutionWidget::~ResolutionWidget() { delete ui; }
