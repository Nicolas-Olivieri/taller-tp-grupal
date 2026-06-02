#ifndef ACTIONDTO_H
#define ACTIONDTO_H

#include "common/dto/message.h"

#include "chatmessage.h"
#include "despawn.h"

enum class ActionType : uint8_t { DESPAWN, MESSAGE };

struct ActionDTO: public ProtocolMessageDTO {
    ActionType action;

    DespawnDTO despawn;
    ChatMessageDTO chat_message;

    // TODO: REVISAR CONSTRUCTOR DEPENDIENDO DE COMO SE MANEJEN LAS ACTION EN EL FUTURO.
    // pueden llegar a no necesitar el ActionType dependiendo del contenido.
    //    explicit ActionDTO(ActionType action);

    explicit ActionDTO(const DespawnDTO& despawn);

    explicit ActionDTO(const ChatMessageDTO& message);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};

#endif  // ACTIONDTO_H
