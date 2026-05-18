#ifndef MOVEEVENT_H
#define MOVEEVENT_H

#include "common/direction.h"

#include "event.h"

struct MoveEventDTO: public EventDTO {
    Direction direction;

    explicit MoveEventDTO(Direction direction);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};

#endif  // MOVEEVENT_H
