#ifndef LOBBYWINDOW_H
#define LOBBYWINDOW_H

#include <QMainWindow>
#include <string>

#include <SDL2pp/Mixer.hh>
#include <SDL2pp/Music.hh>
#include <SDL2pp/SDL.hh>
#include <ui_creator_window.h>
#include <ui_login_window.h>

#include "common/socket.h"

#include "creator_window.h"

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
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

private slots:
    void connect_match();

    void send_creation_data(const CreatePlayerDTO& player_data);

private:
    Ui::LoginWindow* ui;
    std::optional<Socket> socket;
    std::string username;

    QPoint drag_offset;

    std::optional<SDL2pp::SDL> sdl;
    std::optional<SDL2pp::Mixer> mixer;
    std::optional<SDL2pp::Music> background_music;

    bool can_create_session();

    void init_music(const std::string& audio_path, uint8_t volume);

    void set_background_music(const std::string& audio_path, uint8_t volume);
};

#endif  // LOBBYWINDOW_H
