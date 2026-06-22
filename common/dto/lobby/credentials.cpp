#include "credentials.h"

#include "common/protocol/serializer.h"

CredentialsDTO::CredentialsDTO(const std::string& username): username(username), password("default") {}

CredentialsDTO::CredentialsDTO(const std::string& username, const std::string& password):
        username(username), password(password) {}

size_t CredentialsDTO::message_size() const {
    return sizeof(uint16_t) + username.size() + sizeof(uint16_t) + password.size();
}

void CredentialsDTO::accept(Serializer& serializer) const { return serializer.serialize(*this); }
