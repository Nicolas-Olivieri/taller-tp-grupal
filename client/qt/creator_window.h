#ifndef CREATOR_WINDOW_H
#define CREATOR_WINDOW_H

#include <QLabel>
#include <QWidget>

#include "common/dto/lobby/create_player.h"
#include "widgets/preview_widget.h"
#include "widgets/selector_widget.h"

namespace Ui {
class CreatorWindow;
}

class CreatorWindow: public QWidget {
    Q_OBJECT

public:
    explicit CreatorWindow(const QString& username, QWidget* parent = nullptr);

    ~CreatorWindow();

signals:
    void finish_creation(CreatePlayerDTO player_data);

    void exit_creator();

private:
    Ui::CreatorWindow* ui;
    SelectorWidget* head_selector;
    SelectorWidget* body_selector;
    PreviewWidget* preview;

    QHash<QString, QHash<int, QString>> skins_paths;
    QHash<QString, QVector<int>> skin_ranges;

    void load_skins_data();

    void start_game();

    void exit_window();

    bool validate_data();
};

#endif  // CREATOR_WINDOW_H
