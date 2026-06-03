#ifndef LOBBYWINDOW_H
#define LOBBYWINDOW_H

#include <QMainWindow>
#include <string>

#include "common/socket.h"

namespace Ui {
class LobbyWindow;
}

class LobbyWindow: public QMainWindow {
    Q_OBJECT

public:
    explicit LobbyWindow(QWidget* parent = nullptr);

    Socket get_socket();

    std::string get_username();

    ~LobbyWindow();

private slots:
    void conect_match();

private:
    Ui::LobbyWindow* ui;
    std::optional<Socket> socket;
    std::string username;

    bool can_create_session();
};

#endif  // LOBBYWINDOW_H
