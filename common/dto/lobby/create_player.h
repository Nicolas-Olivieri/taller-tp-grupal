#ifndef CREATE_APPEARANCE_H
#define CREATE_APPEARANCE_H
#include "common/dto/message.h"
#include "common/dto/snapshot/info/appearance.h"


struct CreatePlayerDTO: public ProtocolMessageDTO {
    AppearanceDTO appearance;
    uint8_t archetype;
    uint8_t race;

    CreatePlayerDTO(uint8_t body, uint8_t head, uint8_t archetype, uint8_t race);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};


#endif  // CREATE_APPEARANCE_H
