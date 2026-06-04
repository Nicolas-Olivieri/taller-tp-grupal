#include "selector_widget.h"

#include <iostream>
#include <QToolButton>

#include "ui_selector_widget.h"

SelectorWidget::SelectorWidget(const QString& label, const int min, const int max, QWidget* parent):
    QWidget(parent), ui(new Ui::SelectorWidget), label(label){

    ui->setupUi(this);

    ui->label->setText(label);
    ui->optionBox->setMinimum(min);
    ui->optionBox->setMaximum(max);

    connect(ui->btnLeft, &QPushButton::clicked, this, &SelectorWidget::on_left_clicked);
    connect(ui->btnRight, &QPushButton::clicked, this, &SelectorWidget::on_right_clicked);
}

void SelectorWidget::on_left_clicked() {
    const int min = ui->optionBox->minimum();
    const int max = ui->optionBox->maximum();
    const int value = ui->optionBox->value();

    if (value > min) {
        ui->optionBox->setValue(value - 1);
    } else {
        ui->optionBox->setValue(max);
    }
    emit option_changed(ui->optionBox->value());
}

void SelectorWidget::on_right_clicked() {
    const int min = ui->optionBox->minimum();
    const int max = ui->optionBox->maximum();
    const int value = ui->optionBox->value();

    if (value < max) {
        ui->optionBox->setValue(value + 1);
    } else {
        ui->optionBox->setValue(min);
    }
    emit option_changed(ui->optionBox->value());
}

SelectorWidget::~SelectorWidget() { delete ui; }
