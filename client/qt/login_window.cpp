#include "login_window.h"

#include <QMovie>

#include <string>
#include <utility>

#include "common/dto/lobby/credentials.h"
#include "common/liberror.h"
#include "common/protocol/protocol.h"
#include "common/socket.h"

#include "ui_login_window.h"

LoginWindow::LoginWindow(QWidget* parent): QMainWindow(parent), ui(new Ui::LoginWindow) {
    ui->setupUi(this);

    // Configuro la pantalla de LOGIN
    qApp->setStyleSheet("QToolTip { background-color: #1a0f05; color: #f7e5b3; border: 2px solid #c9a87c; "
                        "border-radius: 8px; padding: 6px 12px; }");
    const auto backgroundMovie = new QMovie(":/images/login/animacion.gif");
    ui->animation->setMovie(backgroundMovie);
    backgroundMovie->start();
    ui->conn_err->hide();
    ui->name_err->hide();

    creator = new CreateWindow(this);
    setCentralWidget(creator);

    connect(ui->connectBtn, &QPushButton::clicked, this, &LoginWindow::conect_match);
}

void LoginWindow::conect_match() {
    ui->conn_err->hide();
    ui->name_err->hide();

    if (!can_create_session())
        return;

    username = ui->user->text().trimmed().toStdString();

    Protocol protocol(socket.value());

    const CredentialsDTO message(username);

    protocol.send(message);
    close();

    // TODO falta el download del mapa
}

Socket LoginWindow::get_socket() {
    if (!socket) {
        throw std::runtime_error("No se pudo conectar correctamente con el servidor");
    }
    return std::move(socket.value());
}

std::string LoginWindow::get_username() { return username; }

bool LoginWindow::can_create_session() {
    // TODO agregar validación de usuario desde el servidor y modificar el mensaje de error en caso de no
    // existir el user
    if (ui->user->text().isEmpty()) {
        ui->name_err->show();
        return false;
    }

    // TODO borrar, es el default
    // if (ui->host->text().isEmpty() || ui->port->text().isEmpty()) {
    //     socket.emplace("127.0.0.1", "5050");
    //     return true;
    // }

    const char* hostname = ui->host->text().trimmed().toUtf8().constData();
    const char* servname = ui->port->text().trimmed().toUtf8().constData();

    try {
        socket.emplace(hostname, servname);
    } catch (const std::exception& error) {
        ui->conn_err->show();
        ui->host->clear();
        ui->port->clear();
        return false;
    }
    return true;
}

LoginWindow::~LoginWindow() { delete ui; }
