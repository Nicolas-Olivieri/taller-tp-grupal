#ifndef MESSAGE_H
#define MESSAGE_H

#include <cstddef>
#include <cstdint>
#include <vector>

// Forward declaration para evitar inclusión cíclica al implementar double
// dispatch
class Serializer;

enum class Message : uint8_t { CREDENTIALS, COMMAND, SNAPSHOT };

struct ProtocolMessageDTO {
    virtual size_t message_size() const = 0;
    // Cada DTO tiene que implementar este para hacer el llamado de Serializer
    // con *this
    virtual void accept(Serializer& serializer) const = 0;  // NOLINT
    virtual ~ProtocolMessageDTO() = default;
};

#endif  // MESSAGE_H
