#ifndef LOBBYWINDOW_H
#define LOBBYWINDOW_H

#include <QMainWindow>
#include <string>
#include <ui_creator_window.h>
#include <ui_login_window.h>

#include "creator_window.h"
#include "common/socket.h"

namespace Ui {
class LoginWindow;
}

class LoginWindow: public QMainWindow {
    Q_OBJECT

public:
    explicit LoginWindow(QWidget* parent = nullptr);

    Socket get_socket();

    std::string get_username();

    ~LoginWindow();

protected:
    // Para permitir el borde custom
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    void conect_match();

    void send_creation_data(const CreatePlayerDTO &player_data);

private:
    Ui::LoginWindow* ui;
    std::optional<Socket> socket;
    std::string username;

    QPoint drag_offset;

    bool can_create_session();
};

#endif  // LOBBYWINDOW_H
