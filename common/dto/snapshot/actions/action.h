#ifndef ACTIONDTO_H
#define ACTIONDTO_H

#include "common/dto/message.h"
#include "common/dto/snapshot/actions/action_types/act_despawn/despawn.h"
#include "common/dto/snapshot/actions/action_types/act_message/chatmessage.h"
#include "common/dto/snapshot/actions/action_types/act_resurrection/resurrection.h"

enum class ActionType : uint8_t { DESPAWN, MESSAGE, RESURRECTION };

struct ActionDTO: public ProtocolMessageDTO {
    ActionType action;

    DespawnDTO despawn;
    ChatMessageDTO chat_message;
    ResurrectionDTO resurrection;

    // TODO: REVISAR CONSTRUCTOR DEPENDIENDO DE COMO SE MANEJEN LAS ACTION EN EL FUTURO.
    // pueden llegar a no necesitar el ActionType dependiendo del contenido.
    //    explicit ActionDTO(ActionType action);

    explicit ActionDTO(const DespawnDTO& despawn);

    explicit ActionDTO(const ChatMessageDTO& message);

    explicit ActionDTO(const ResurrectionDTO& resurrection);

    // Modificarlo al agregar nuevas actions
    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};

#endif  // ACTIONDTO_H
