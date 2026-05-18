#ifndef DESERIALIZER_H
#define DESERIALIZER_H

#include <cstdint>
#include <string>

#include "common/socket.h"

class Deserializer {
private:
    Socket& socket;

public:
    explicit Deserializer(Socket& socket);  // NOLINT

    std::string recv_string();

    uint8_t recv_uint8();

    uint16_t recv_uint16();
};

#endif  // DESERIALIZER_H
