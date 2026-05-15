#include "lobby.h"
#include "common/socket.h"
#include "client/client_protocol.h"

Lobby::Lobby() : QMainWindow(nullptr) {

    this->setMinimumSize(640, 480);
    // Configuración de la interfaz gráfica (Widgets y Layouts)
    _centralWidget = new QWidget(this);
    layout = new QHBoxLayout(_centralWidget);

    inputIP = new QLineEdit(this);
    inputIP->setPlaceholderText("Escribe una dirección IP");
    inputPort = new QLineEdit(this);
    inputPort->setPlaceholderText("Escribe un puerto");

    sendButton = new QPushButton("Conectarse", this);

    layout->addWidget(inputIP);
    layout->addWidget(inputPort);
    layout->addWidget(sendButton);
    setCentralWidget(_centralWidget);

    // Conexión Signal-Slot: El motor de la comunicación en Qt
    // Señal 'clicked()' del botón --> Slot 'agregarTarea()' de esta ventana
    connect(sendButton, &QPushButton::clicked, this, &Lobby::conectMatch);
}

void Lobby::conectMatch() {
    const char* hostname = inputIP->text().trimmed().toUtf8().constData();
    const char* servname = inputPort->text().trimmed().toUtf8().constData();

    socket.emplace(hostname, servname);

    ClientProtocol protocol(socket.value());

    const DataDTO msg = {Command::HANDSHAKE, "Hola mundo! Atte. Cliente"};
    protocol.send_msg(msg);

    close();
}

Socket Lobby::finish() {
    if (!socket) {
        throw std::runtime_error("No se pudo conectar correctamente con el servidor");
    }
    return std::move(socket.value());
}
