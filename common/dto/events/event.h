#ifndef EVENTDTO_H
#define EVENTDTO_H

#include "common/dto/message.h"

#include "command.h"

// Clase base de los eventos. No es abstracta, pero otros eventos con más campos
// van a heredar de ella
struct EventDTO: public ProtocolMessageDTO {
    CommandType command;

    explicit EventDTO(const CommandType& command);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;

    virtual ~EventDTO() = default;  // obligatorio por herencia
};

#endif  // EVENTDTO_H
