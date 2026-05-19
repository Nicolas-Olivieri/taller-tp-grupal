#include "lobby.h"

#include <QMessageBox>
#include <utility>

#include "common/dto/lobby/credentials.h"
#include "common/liberror.h"
#include "common/protocol/protocol.h"
#include "common/socket.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480


Lobby::Lobby(): QMainWindow(nullptr) {

    this->setMinimumSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    // Configuración de la interfaz gráfica (Widgets y Layouts)
    central_widget = new QWidget(this);
    layout = new QVBoxLayout(central_widget);

    input_ip = new QLineEdit(this);
    input_ip->setPlaceholderText("Escribe una dirección IP");
    input_port = new QLineEdit(this);
    input_port->setPlaceholderText("Escribe un puerto");
    input_username = new QLineEdit(this);
    input_username->setPlaceholderText("Escribe tu username");

    send_button = new QPushButton("Conectarse", this);

    layout->addWidget(input_ip);
    layout->addWidget(input_port);
    layout->addWidget(input_username);
    layout->addWidget(send_button);

    setCentralWidget(central_widget);

    this->setStyleSheet(style.c_str());

    // Conexión Signal-Slot: El motor de la comunicación en Qt
    // Señal 'clicked()' del botón --> Slot 'agregarTarea()' de esta ventana
    connect(send_button, &QPushButton::clicked, this, &Lobby::conectMatch);
}

void Lobby::conectMatch() {
    if (!can_create_socket())
        return;

    username = input_username->text().trimmed().toStdString();

    Protocol protocol(socket.value());

    const CredentialsDTO message(username);

    protocol.send(message);
    close();

    // TODO falta el download del mapa
}

Socket Lobby::get_socket() {
    if (!socket) {
        throw std::runtime_error(
                "No se pudo conectar correctamente con el servidor");
    }
    return std::move(socket.value());
}

std::string Lobby::get_username() { return username; }

bool Lobby::can_create_socket() {
    // TODO borrar, es el default
    if (input_ip->text().isEmpty() || input_port->text().isEmpty()) {
        socket.emplace("127.0.0.1", "5050");
        return true;
    }

    const char* hostname = input_ip->text().trimmed().toUtf8().constData();
    const char* servname = input_port->text().trimmed().toUtf8().constData();

    try {
        socket.emplace(hostname, servname);
    } catch (const std::exception& error) {
        std::string error_txt(error.what());
        std::string warning_txt =
                "Se obtuvo el siguiente error: " + error_txt +
                "\nVerifique que la dirección IP y Puerto ingresados "
                "correspondan a un servidor de Argentum";
        QMessageBox::warning(this, "Error en la conexión", warning_txt.c_str());
        input_ip->clear();
        input_port->clear();
        return false;
    }
    return true;
}
