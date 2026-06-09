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


LoginWindow::LoginWindow(AudioManager& audio_manager, QWidget* parent):
        QMainWindow(parent), ui(new Ui::LoginWindow), audio_manager(audio_manager), allow_close(false), force_close(false) {
    ui->setupUi(this);

    // Seteo Stacked Widget para ambas pantallas
    stacked_widget = new QStackedWidget(this);
    QWidget* loginPage = takeCentralWidget();
    stacked_widget->addWidget(loginPage);
    QWidget* creatorContainer = new QWidget();
    stacked_widget->addWidget(creatorContainer);
    setCentralWidget(stacked_widget);

    // Seteo borde de ventana custom
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);
    connect(ui->btnClose, &QPushButton::clicked, this, &LoginWindow::exit_window);
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
    audio_manager.play_music(MusicTrack::LOGIN);
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
        QWidget* container = stacked_widget->widget(1);

        creator_window = new CreatorWindow(QString::fromStdString(username), container);
        auto* layout = new QHBoxLayout(container);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(creator_window);

        connect(creator_window, &CreatorWindow::finish_creation, this, &LoginWindow::send_creation_data);
        connect(creator_window, &CreatorWindow::exit_creator, this, &LoginWindow::exit_window);

        stacked_widget->setCurrentIndex(1);

        audio_manager.play_music(MusicTrack::CREATOR);
    } else {
        allow_close = true;
        audio_manager.play_music(MusicTrack::FOREST);
        close();
    }
}

void LoginWindow::send_creation_data(const CreatePlayerDTO& player_data) {
    audio_manager.play_music(MusicTrack::FOREST);
    Protocol protocol(socket.value());
    protocol.send(player_data);

    ExistenceDTO confirmation = protocol.recv_existence();
    if (!confirmation.user_exists) {
        allow_close = true;
        close();
        return;
    }

    stacked_widget->setCurrentIndex(0);
    ui->name_err->setText("Alguien acaba de crearse una cuenta con este nombre. Elegí otro");
    ui->name_err->show();

    socket.reset();
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

void LoginWindow::exit_window() {
    force_close = true;
    allow_close = true;
    close();
}

void LoginWindow::closeEvent(QCloseEvent *event) {
    if (allow_close) {
        event->accept();
        QApplication::quit();
    } else {
        event->ignore();
    }
}

Socket LoginWindow::get_socket() {
    if (!socket) {
        throw std::runtime_error("No se pudo conectar correctamente con el servidor");
    }
    return std::move(socket.value());
}

std::string LoginWindow::get_username() { return username; }

bool LoginWindow::was_forced_close() const {
    return force_close;
}


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
