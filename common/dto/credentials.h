#ifndef CREDENTIALS_H
#define CREDENTIALS_H

#include <string>

#include "common/message.h"

struct CredentialsDTO {
    std::string username;
    std::string password;

    explicit CredentialsDTO(const std::string& username):
            username(username), password("default") {}  // TODO: eliminar esta

    // TODO: usar solo esta firma y eliminar la otra cuando implementemos la
    // lógica con contraseñas
    CredentialsDTO(const std::string& username, const std::string& password):
            username(username), password(password) {}
};

#endif  // CREDENTIALS_H
