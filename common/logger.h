#ifndef LOGGER_H
#define LOGGER_H

#include <exception>

#include <syslog.h>

#define END_MSG "Finalizando ejecución..."

#define SERVER_ID "Argentum-Online-server"
#define CLIENT_ID "Argentum-Online-client"

#define SERVER_INIT "Iniciando el servidor en el puerto: %s"
#define CLIENT_INIT "Iniciando el cliente en la dirección IP:puerto: %s:%s"

#define MSG_FORMAT "%s"
#define EXCEPTION_FORMAT "%s: %s"

#define EXCEPTION_MSG "Una excepción fue capturada"
#define UNKNOWN_EXCEPTION_MSG "Una excepción desconocida fue capturada"


class Logger {
public:
    Logger() = default;

    void open(const char* serv) const;

    void open(const char* host, const char* serv) const;

    void info(const char* msg) const;

    void err(const char* msg, const std::exception& e) const;

    void crit(const char* msg) const;

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    ~Logger();
};


#endif  // LOGGER_H
