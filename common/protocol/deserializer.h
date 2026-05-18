#ifndef DESERIALIZER_H
#define DESERIALIZER_H

#include <cstdint>
#include <string>

#include "common/direction.h"
#include "common/dto/command.h"
#include "common/socket.h"

class Deserializer {
private:
    Socket& socket;

public:
    explicit Deserializer(Socket& socket);  // NOLINT

    std::string recv_string();

    uint8_t recv_uint8();

    uint16_t recv_uint16();

    CommandType recv_command_type();

    Direction recv_direction();
};

#endif  // DESERIALIZER_H
