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

        Socket finish();

        private slots:
        void conectMatch(); // La función que se ejecutará cuando se pulse el botón

        private:
        QWidget* _centralWidget;
        QHBoxLayout* layout;
        QLineEdit* inputIP;
        QLineEdit* inputPort;
        QPushButton* sendButton;

        std::optional<Socket> socket;
};

#endif //TALLER_TP_LOBBY_H
