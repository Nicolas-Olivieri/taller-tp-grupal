#ifndef TALLER_TP_LOBBY_H
#define TALLER_TP_LOBBY_H

#include <QMainWindow>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include "common/socket.h"

class Lobby: public QMainWindow {
        Q_OBJECT // no borrar la macro, tiene sentido aca

        public:
        Lobby();

        Socket get_socket();

        std::string get_username();

        private slots:
        void conectMatch(); // La función que se ejecutará cuando se pulse el botón

        private:
        QWidget* central_widget;
        QVBoxLayout* layout;
        QLineEdit* input_ip;
        QLineEdit* input_port;
        QLineEdit* input_username;
        QPushButton* send_button;

        std::optional<Socket> socket;
        std::string username;

        bool can_create_socket();

        const std::string style = R"(
        QLineEdit {
            border: 1px solid #aaa;
            border-radius: 5px;
            padding: 8px;
            font-size: 16px;
        }
        QPushButton {
            background-color: #2c3e50;
            color: white;
            border-radius: 5px;
            padding: 8px;
            font-weight: bold;
        }
        QPushButton:hover {
                background-color: #1abc9c;
        }
        QPushButton:disabled {
            background-color: #95a5a6;
        }
        )";
};

#endif //TALLER_TP_LOBBY_H
