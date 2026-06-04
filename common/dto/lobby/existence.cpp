#include "existence.h"

#include "common/protocol/serializer.h"

ExistenceDTO::ExistenceDTO(const uint8_t user_exists) : user_exists(user_exists) {}

size_t ExistenceDTO::message_size() const {
    return sizeof(user_exists);
}

void ExistenceDTO::accept(Serializer &serializer) const {
    return serializer.serialize(*this);
}
