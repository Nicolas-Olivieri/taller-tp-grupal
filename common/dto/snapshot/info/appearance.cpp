#include "appearance.h"

#include "common/protocol/serializer.h"

AppearanceDTO::AppearanceDTO(uint8_t body, uint8_t head): body(body), head(head) {}

size_t AppearanceDTO::message_size() const {
    return sizeof(body) + sizeof(head);
}

void AppearanceDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
