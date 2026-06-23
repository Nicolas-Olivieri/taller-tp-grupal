#ifndef TUTORIAL_OVERLAY_H
#define TUTORIAL_OVERLAY_H

#include <QString>
#include <QVector>
#include <QWidget>

namespace Ui {
class TutorialOverlay;
}

struct TutorialStep {
    QWidget* target_widget;
    QString description;
};

class TutorialOverlay: public QWidget {
    Q_OBJECT

public:
    explicit TutorialOverlay(QWidget* parent = nullptr);
    ~TutorialOverlay() override;

    void add_step(QWidget* widget, const QString& text);

    void start();

protected:
    void paintEvent(QPaintEvent* event) override;
    bool eventFilter(QObject* watched, QEvent* event) override;

private slots:
    void on_btnNext_clicked();
    void on_btnPrev_clicked();
    void on_btnClose_clicked();

private:
    Ui::TutorialOverlay* ui;
    QVector<TutorialStep> steps;
    int current_step;

    void updateUI();
};

#endif  // TUTORIAL_OVERLAY_H
