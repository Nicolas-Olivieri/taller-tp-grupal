#ifndef CREDENTIALSDTO_H
#define CREDENTIALSDTO_H

#include <string>

#include "common/dto/message.h"

struct CredentialsDTO: public ProtocolMessageDTO {
    std::string username;
    std::string password;

    explicit CredentialsDTO(
            const std::string& username);  // TODO: eliminar esta

    // TODO: usar solo esta firma y eliminar la otra cuando implementemos la
    // lógica con contraseñas
    CredentialsDTO(const std::string& username, const std::string& password);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};

#endif  // CREDENTIALSDTO_H
