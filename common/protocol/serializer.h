#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <cstdint>
#include <string>
#include <vector>

#include "common/dto/credentials.h"
#include "common/dto/message.h"

class Serializer {
private:
    std::vector<uint8_t>& buffer;
    size_t offset;

    void serialize(const std::string& value);

    void serialize(uint8_t value);

    void serialize(uint16_t value);

public:
    explicit Serializer(std::vector<uint8_t>& buffer);  // NOLINT

    void serialize(const ProtocolMessageDTO& dto);

    void serialize(const CredentialsDTO& credentials);
};

#endif  // SERIALIZER_H
