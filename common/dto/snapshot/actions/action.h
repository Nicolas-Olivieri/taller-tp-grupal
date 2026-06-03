#ifndef ACTIONDTO_H
#define ACTIONDTO_H

#include "action_types/act_death/death.h"
#include "action_types/act_list_items/list_items.h"
#include "common/dto/message.h"
#include "common/dto/snapshot/actions/action_types/act_despawn/despawn.h"
#include "common/dto/snapshot/actions/action_types/act_list/chat_list.h"
#include "common/dto/snapshot/actions/action_types/act_message/chatmessage.h"
#include "common/dto/snapshot/actions/action_types/act_resurrection/resurrection.h"

enum class ActionType : uint8_t { DESPAWN, MESSAGE, RESURRECTION, DEATH, MESSAGE_LIST, LIST_ITEMS };

struct ActionDTO: public ProtocolMessageDTO {
    ActionType action;

    DespawnDTO despawn;
    ChatMessageDTO chat_message;
    ResurrectionDTO resurrection;
    DeathDTO death;
    ChatListDTO list;
    ListItemsDTO items;

    // TODO: REVISAR CONSTRUCTOR DEPENDIENDO DE COMO SE MANEJEN LAS ACTION EN EL FUTURO.
    // pueden llegar a no necesitar el ActionType dependiendo del contenido.
    //    explicit ActionDTO(ActionType action);

    explicit ActionDTO(const DespawnDTO& despawn);

    explicit ActionDTO(const ChatMessageDTO& message);

    explicit ActionDTO(const ResurrectionDTO& resurrection);

    explicit ActionDTO(const DeathDTO& death);

    explicit ActionDTO(const ChatListDTO& list);

    explicit ActionDTO(const ListItemsDTO& items);

    // Modificarlo al agregar nuevas actions
    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};

#endif  // ACTIONDTO_H
