#ifndef LOBBYWINDOW_H
#define LOBBYWINDOW_H

#include <QMainWindow>
#include <string>
#include <ui_create_window.h>
#include <ui_login_window.h>

#include "create_window.h"
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

private slots:
    void conect_match();

    void send_creation_data(const CreatePlayerDTO &player_data);

private:
    Ui::LoginWindow* ui;
    std::optional<Socket> socket;
    std::string username;

    bool can_create_session();
};

#endif  // LOBBYWINDOW_H
