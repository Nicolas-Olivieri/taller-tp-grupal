#include "existence.h"

#include "common/protocol/serializer.h"

ExistenceDTO::ExistenceDTO(const uint8_t user_exists, const uint8_t user_connected):
        user_exists(user_exists), user_connected(user_connected) {}

size_t ExistenceDTO::message_size() const { return sizeof(user_exists) + sizeof(user_connected); }

void ExistenceDTO::accept(Serializer& serializer) const { return serializer.serialize(*this); }
