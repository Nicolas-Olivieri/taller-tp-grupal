#ifndef TYPES_H
#define TYPES_H

#include <cstdint>
#include <string>

enum CommandCode : uint8_t { HANDSHAKE = 0xFF };

// Para este PoC usamos el mismo DTO para server y cliente
// En la implementación del proyecto probablemente terminemos con al menos un
// (1) DTO para el server y otro para el cliente (ya sea porque envían info.
// distinta o para diferenciar los enum Command)
struct DataDTO {
    CommandCode command;
    std::string msg;
};

#endif
