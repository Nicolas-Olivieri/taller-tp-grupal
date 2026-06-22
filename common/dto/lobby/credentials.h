#ifndef CREDENTIALSDTO_H
#define CREDENTIALSDTO_H

#include <string>

#include "common/dto/message.h"

// TODO: sacar la password y el constructor que la usa
struct CredentialsDTO: public ProtocolMessageDTO {
    std::string username;

    explicit CredentialsDTO(const std::string& username);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};

#endif  // CREDENTIALSDTO_H
