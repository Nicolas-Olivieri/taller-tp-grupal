#ifndef ACTIONDTO_H
#define ACTIONDTO_H

#include "common/dto/message.h"

enum class ActionType : uint8_t { PLACEHOLDER };  // TODO: eliminar PLACEHOLDER

struct ActionDTO: public ProtocolMessageDTO {
    ActionType action;

    // TODO: REVISAR CONSTRUCTOR DEPENDIENDO DE COMO SE MANEJEN LAS ACTION EN EL FUTURO.
    // pueden llegar a no necesitar el ActionType dependiendo del contenido.
    explicit ActionDTO(ActionType action);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};

#endif  // ACTIONDTO_H
