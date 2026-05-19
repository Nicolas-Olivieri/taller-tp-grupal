#include "appearance.h"

#include "common/protocol/serializer.h"

AppearanceDTO::AppearanceDTO(uint8_t body, uint8_t head,
                             const std::string& name):
        body(body), head(head), name(name) {}

size_t AppearanceDTO::message_size() const {
    return sizeof(body) + sizeof(head) + sizeof(uint16_t) + name.size();
}

void AppearanceDTO::accept(Serializer& serializer) const {
    serializer.serialize(*this);
}
