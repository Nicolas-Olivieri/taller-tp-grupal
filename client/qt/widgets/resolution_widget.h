#ifndef RESOLUTION_WIDGET_H
#define RESOLUTION_WIDGET_H

#include <QSpinBox>
#include <QWidget>
#include <string>

namespace Ui {
class ResolutionWidget;
}

class ResolutionWidget: public QWidget {
    Q_OBJECT

public:
    explicit ResolutionWidget(QWidget* parent = nullptr);

    std::string get_resolution() const;

    ~ResolutionWidget();

private:
    Ui::ResolutionWidget* ui;
    QList<QString> resolution_values;
    int curr_opt;

    void on_left_clicked();
    void on_right_clicked();
};

#endif  // RESOLUTION_WIDGET_H
