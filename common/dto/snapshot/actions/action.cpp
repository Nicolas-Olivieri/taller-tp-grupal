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
        case ActionType::HEAL:
            return base + heal.message_size();
        case ActionType::MESSAGE:
            return base + chat_message.message_size();
        case ActionType::MEDITATION:
            return base + meditation.message_size();
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
        case ActionType::CLAN_MESSAGE:
            return base + clan_msg.message_size();
        default:
            throw std::runtime_error("ActionDTO Descubrió que tiene un tipo de acción desconocido al "
                                     "calcular su message_size");
    }
}

void ActionDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }

ActionDTO::ActionDTO(const HealDTO& heal): action(ActionType::HEAL), heal(heal) {}

ActionDTO::ActionDTO(const AttackDTO& attack): action(ActionType::ATTACK), attack(attack) {}

ActionDTO::ActionDTO(const DespawnDTO& despawn): action(ActionType::DESPAWN), despawn(despawn) {}

ActionDTO::ActionDTO(const ChatMessageDTO& message): action(ActionType::MESSAGE), chat_message(message) {}

ActionDTO::ActionDTO(const MeditationDTO& meditation):
        action(ActionType::MEDITATION), meditation(meditation) {}

ActionDTO::ActionDTO(const ResurrectionDTO& resurrection):
        action(ActionType::RESURRECTION), resurrection(resurrection) {}

ActionDTO::ActionDTO(const DeathDTO& death): action(ActionType::DEATH), death(death) {}

ActionDTO::ActionDTO(const ChatListDTO& list): action(ActionType::MESSAGE_LIST), list(list) {}

ActionDTO::ActionDTO(const ListItemsDTO& items): action(ActionType::LIST_ITEMS), items(items) {}

ActionDTO::ActionDTO(const ListBankDTO& bank): action(ActionType::LIST_BANK), bank(bank) {}

ActionDTO::ActionDTO(const ClanMessageDTO& clan_msg): action(ActionType::CLAN_MESSAGE), clan_msg(clan_msg) {}

ActionDTO::ActionDTO(const ClanMessageDTO& clan_msg):
        action(ActionType::CLAN_MESSAGE),
        despawn(""),
        chat_message(MessageType::SYSTEM, "", "", ""),
        resurrection("", {}),
        death(""),
        list(MessageType::SYSTEM, {}, ""),
        items(),
        bank(),
        clan_msg(clan_msg) {}
