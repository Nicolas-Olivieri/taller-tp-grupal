#include "action.h"

#include <stdexcept>

#include "common/protocol/serializer.h"

// TODO: este método debería cambiar su comportamiento dependiendo del
// ActionType, considerar opciones de implementación
size_t ActionDTO::message_size() const {
    const auto base = sizeof(action);

    switch (action) {
        case ActionType::ATTACK:
            return base + attack.message_size();
        case ActionType::DESPAWN:
            return base + despawn.message_size();
        case ActionType::MESSAGE:
            return base + chat_message.message_size();
        case ActionType::RESURRECTION:
            return base + resurrection.message_size();
        case ActionType::DEATH:
            return base + death.message_size();
        case ActionType::MESSAGE_LIST:
            return base + list.message_size();
        case ActionType::LIST_ITEMS:
            return base + items.message_size();
        case ActionType::LIST_BANK:
            return base + bank.message_size();
        default:
            throw std::runtime_error("ActionDTO Descubrió que tiene un tipo de acción desconocido al "
                                     "calcular su message_size");
    }
}

void ActionDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }

ActionDTO::ActionDTO(const AttackDTO& attack):
        action(ActionType::ATTACK),
        attack(attack),
        despawn(""),
        chat_message(MessageType::SYSTEM, "", "", ""),
        resurrection("", {}),
        death(""),
        list(MessageType::SYSTEM, {}, ""),
        items() {}

ActionDTO::ActionDTO(const DespawnDTO& despawn):
        action(ActionType::DESPAWN),
        despawn(despawn),
        chat_message(MessageType::SYSTEM, "", "", ""),
        resurrection("", {}),
        death(""),
        list(MessageType::SYSTEM, {}, ""),
        items(),
        bank() {}

ActionDTO::ActionDTO(const ChatMessageDTO& message):
        action(ActionType::MESSAGE),
        despawn(""),
        chat_message(message),
        resurrection("", {}),
        death(""),
        list(MessageType::SYSTEM, {}, ""),
        items(),
        bank() {}

ActionDTO::ActionDTO(const ResurrectionDTO& resurrection):
        action(ActionType::RESURRECTION),
        despawn(""),
        chat_message(MessageType::SYSTEM, "", "", ""),
        resurrection(resurrection),
        death(""),
        list(MessageType::SYSTEM, {}, ""),
        items(),
        bank() {}

ActionDTO::ActionDTO(const DeathDTO& death):
        action(ActionType::DEATH),
        despawn(""),
        chat_message(MessageType::SYSTEM, "", "", ""),
        resurrection("", {}),
        death(death),
        list(MessageType::SYSTEM, {}, ""),
        items(),
        bank() {}

ActionDTO::ActionDTO(const ChatListDTO& list):
        action(ActionType::MESSAGE_LIST),
        despawn(""),
        chat_message(MessageType::SYSTEM, "", "", ""),
        resurrection("", {}),
        death(""),
        list(list),
        items(),
        bank() {}

ActionDTO::ActionDTO(const ListItemsDTO& items):
        action(ActionType::LIST_ITEMS),
        despawn(""),
        chat_message(MessageType::SYSTEM, "", "", ""),
        resurrection("", {}),
        death(""),
        list(MessageType::SYSTEM, {}, ""),
        items(items),
        bank() {}

ActionDTO::ActionDTO(const ListBankDTO& bank):
        action(ActionType::LIST_BANK),
        despawn(""),
        chat_message(MessageType::SYSTEM, "", "", ""),
        resurrection("", {}),
        death(""),
        list(MessageType::SYSTEM, {}, ""),
        items(),
        bank(bank) {}
