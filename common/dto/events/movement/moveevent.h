#ifndef MOVEEVENTDTO_H
#define MOVEEVENTDTO_H

#include "common/dto/events/event.h"

struct MoveEventDTO: public EventDTO {
    Direction direction;

    explicit MoveEventDTO(Direction direction);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};

#endif  // MOVEEVENTDTO_H
