#ifndef SELECTOR_WIDGET_H
#define SELECTOR_WIDGET_H

#include <QWidget>

namespace Ui {
class SelectorWidget;
}

class SelectorWidget: public QWidget {
    Q_OBJECT

public:
    SelectorWidget(const QString& label, int min, int max, QWidget* parent = nullptr);

    ~SelectorWidget();

    uint8_t get_value() const;

signals:
    void option_changed(int new_id);

private:
    Ui::SelectorWidget* ui;
    QString label;

    void on_left_clicked();
    void on_right_clicked();
};

#endif  // SELECTOR_WIDGET_H
