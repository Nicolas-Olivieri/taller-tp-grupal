#ifndef CREATE_WINDOW_H
#define CREATE_WINDOW_H

#include <QButtonGroup>
#include <QLabel>
#include <QPushButton>
#include <QWidget>

#include "preview_widget.h"
#include "selector_widget.h"
#include "common/socket.h"
#include "common/dto/lobby/create_player.h"

namespace Ui {
class CreateWindow;
}

class CreateWindow: public QWidget {
    Q_OBJECT

public:
    explicit CreateWindow(const QString& username, QWidget* parent = nullptr);

    ~CreateWindow();

signals:
    void finish_creation(CreatePlayerDTO player_data);

private:
    Ui::CreateWindow* ui;
    SelectorWidget* head_selector;
    SelectorWidget* body_selector;
    PreviewWidget* preview;

    QHash<QString, QHash<int, QString>> skins_paths;
    QHash<QString, QVector<int>> skin_ranges;

    void load_skins_data();

    void start_game();

    bool validate_data();
};

#endif  // CREATE_WINDOW_H
