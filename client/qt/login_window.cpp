#include "login_window.h"

#include <QMainWindow>
#include <QMoveEvent>
#include <QMovie>
#include <iostream>
#include <string>
#include <utility>

#include "common/dto/lobby/credentials.h"
#include "common/liberror.h"
#include "common/protocol/protocol.h"
#include "common/socket.h"

#include "ui_login_window.h"

LoginWindow::LoginWindow(QWidget* parent): QMainWindow(parent), ui(new Ui::LoginWindow) {
    ui->setupUi(this);

    // Seteo borde de ventana custom
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    connect(ui->btnClose, &QPushButton::clicked, this, &QWidget::close);
    connect(ui->btnMinimize, &QPushButton::clicked, this, &QWidget::showMinimized);

    // Configuro la pantalla de LOGIN
    qApp->setStyleSheet("QToolTip { background-color: #1a0f05; color: #f7e5b3; border: 2px solid #c9a87c; "
                        "border-radius: 8px; padding: 6px 12px; }");
    const auto backgroundMovie = new QMovie(":/images/login/animacion.gif");
    ui->animation->setMovie(backgroundMovie);
    backgroundMovie->start();
    ui->conn_err->hide();
    ui->name_err->hide();

    connect(ui->connectBtn, &QPushButton::clicked, this, &LoginWindow::connect_match);
}

void LoginWindow::connect_match() {
    ui->conn_err->hide();
    ui->name_err->hide();

    if (!can_create_session())
        return;

    username = ui->user->text().trimmed().toStdString();

    Protocol protocol(socket.value());

    const CredentialsDTO message(username);
    protocol.send(message);

    const ExistenceDTO confirmation = protocol.recv_existence();
    if (confirmation.user_connected) {
        ui->name_err->setText("El usuario ya está conectado");
        ui->name_err->show();
        socket.reset();
        return;
    }

    if (!confirmation.user_exists) {
        const auto creator = new CreatorWindow(QString::fromStdString(username));
        connect(creator, &CreatorWindow::finish_creation, this, &LoginWindow::send_creation_data);

        setCentralWidget(creator);

    } else {
        close();
    }
}

void LoginWindow::send_creation_data(const CreatePlayerDTO& player_data) {
    Protocol protocol(socket.value());

    protocol.send(player_data);

    close();
}

bool LoginWindow::can_create_session() {
    if (ui->user->text().isEmpty()) {
        ui->name_err->setText("Ingresá un nombre de usuario");
        ui->name_err->show();
        return false;
    }

    // TODO borrar, es el default
    if (ui->host->text().isEmpty() || ui->port->text().isEmpty()) {
        socket.emplace("127.0.0.1", "5050");
        return true;
    }

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

Socket LoginWindow::get_socket() {
    if (!socket) {
        throw std::runtime_error("No se pudo conectar correctamente con el servidor");
    }
    return std::move(socket.value());
}

std::string LoginWindow::get_username() { return username; }


void LoginWindow::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        drag_offset = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
}

void LoginWindow::mouseMoveEvent(QMouseEvent* event) {
    if (event->buttons() & Qt::LeftButton) {
        move(event->globalPosition().toPoint() - drag_offset);
        event->accept();
    }
}

LoginWindow::~LoginWindow() { delete ui; }
