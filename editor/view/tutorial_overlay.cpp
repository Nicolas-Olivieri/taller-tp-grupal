#include "tutorial_overlay.h"

#include <QEvent>
#include <QPainter>

#include <qpainterpath.h>

#include "ui_tutorialoverlay.h"

#define PADDING 4

TutorialOverlay::TutorialOverlay(QWidget* parent):
        QWidget(parent), ui(new Ui::TutorialOverlay), current_step(0) {
    ui->setupUi(this);

    // Setea atributos y lo vincula con el padre en caso de resize
    setAttribute(Qt::WA_TransparentForMouseEvents, false);
    setAttribute(Qt::WA_NoSystemBackground);
    parent->installEventFilter(this);
    resize(parent->size());

    hide();
}

TutorialOverlay::~TutorialOverlay() { delete ui; }

void TutorialOverlay::add_step(QWidget* widget, const QString& text) { steps.append({widget, text}); }

void TutorialOverlay::start() {
    if (steps.isEmpty())
        return;

    current_step = 0;
    updateUI();
    show();
    raise();
}

void TutorialOverlay::updateUI() {
    if (current_step < 0 || current_step >= steps.size()) {
        hide();
        return;
    }

    // Configura el boton
    ui->btnPrev->setEnabled(current_step > 0);
    if (current_step == steps.size() - 1) {
        ui->btnNext->setText("Finalizar");
    } else {
        ui->btnNext->setText("Siguiente");
    }

    // Configura el texto
    TutorialStep step = steps[current_step];
    ui->step_description->setText(step.description);
    QRect targetRect = step.target_widget->rect();
    QPoint localPos = this->mapFromGlobal(step.target_widget->mapToGlobal(QPoint(0, 0)));
    ui->dialogFrame->adjustSize();

    // Configura posición del overlay
    int dialogX = localPos.x();
    int dialogY = localPos.y() + targetRect.height() + 10;
    if (dialogX + ui->dialogFrame->width() > width()) {
        dialogX = width() - ui->dialogFrame->width() - 10;
    }
    if (dialogY + ui->dialogFrame->height() > height()) {
        dialogY = localPos.y() - ui->dialogFrame->height() - 10;
    }
    ui->dialogFrame->move(dialogX, dialogY);

    update();
}

void TutorialOverlay::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPainterPath fullPath;
    fullPath.addRect(rect());

    if (current_step < 0 || current_step >= steps.size()) {
        return;
    }

    // Defino rect del boton a remarcar
    const QWidget* target = steps[current_step].target_widget;
    QRect target_rect = target->rect();
    const QPoint local_pos = this->mapFromGlobal(target->mapToGlobal(QPoint(0, 0)));
    target_rect.moveTo(local_pos);
    target_rect.adjust(-PADDING, -PADDING, PADDING, PADDING);

    // Defino el hueco a borrar en el overlay para que se vea el boton
    QPainterPath hole_path;
    hole_path.addRect(target_rect);
    const QPainterPath overlay_path = fullPath.subtracted(hole_path);
    painter.fillPath(overlay_path, QColor(0, 0, 0, 150));

    // Dibujo el borde de color para remarcar el boton
    const QColor highlight_color = palette().color(QPalette::Highlight);
    painter.setPen(QPen(highlight_color, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawRect(target_rect);
}

bool TutorialOverlay::eventFilter(QObject* watched, QEvent* event) {
    if (watched == parentWidget() && event->type() == QEvent::Resize) {
        resize(parentWidget()->size());
        if (isVisible())
            updateUI();
    }
    return QWidget::eventFilter(watched, event);
}

void TutorialOverlay::on_btnNext_clicked() {
    if (current_step < steps.size() - 1) {
        current_step++;
        updateUI();
    } else {
        hide();
    }
}

void TutorialOverlay::on_btnPrev_clicked() {
    if (current_step > 0) {
        current_step--;
        updateUI();
    }
}

void TutorialOverlay::on_btnClose_clicked() { hide(); }
