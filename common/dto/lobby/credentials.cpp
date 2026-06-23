#include "credentials.h"

#include "common/protocol/serializer.h"

CredentialsDTO::CredentialsDTO(const std::string& username): username(username) {}

size_t CredentialsDTO::message_size() const { return sizeof(uint16_t) + username.size(); }

void CredentialsDTO::accept(Serializer& serializer) const { return serializer.serialize(*this); }
