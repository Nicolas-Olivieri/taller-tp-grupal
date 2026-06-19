#ifndef ACTION_DTO_H
#define ACTION_DTO_H

#include "action_types/act_clan_accept/clan_accept.h"
#include "action_types/act_clan_found/clan_found.h"
#include "action_types/act_clan_leave/clan_leave.h"
#include "action_types/act_meditation/meditation.h"
#include "common/dto/message.h"
#include "common/dto/snapshot/actions/action_types/act_attack/attack.h"
#include "common/dto/snapshot/actions/action_types/act_clan_msg/clan_message.h"
#include "common/dto/snapshot/actions/action_types/act_death/death.h"
#include "common/dto/snapshot/actions/action_types/act_despawn/despawn.h"
#include "common/dto/snapshot/actions/action_types/act_heal/heal.h"
#include "common/dto/snapshot/actions/action_types/act_inventory_list/inventory_list.h"
#include "common/dto/snapshot/actions/action_types/act_list/chat_list.h"
#include "common/dto/snapshot/actions/action_types/act_list_bank/list_bank.h"
#include "common/dto/snapshot/actions/action_types/act_list_items/list_items.h"
#include "common/dto/snapshot/actions/action_types/act_message/chatmessage.h"
#include "common/dto/snapshot/actions/action_types/act_resurrection/resurrection.h"

enum class ActionType : uint8_t {
    ATTACK,
    DESPAWN,
    HEAL,
    MEDITATION,
    MESSAGE,
    RESURRECTION,
    DEATH,
    MESSAGE_LIST,
    LIST_ITEMS,
    LIST_BANK,
    CLAN_MESSAGE,
    CLAN_ACCEPT,
    CLAN_FOUND,
    CLAN_LEAVE,
    INVENTORY_LIST,
};

struct ActionDTO: public ProtocolMessageDTO {
    ActionType action;

    AttackDTO attack;
    DespawnDTO despawn;
    HealDTO heal;
    ChatMessageDTO chat_message;
    MeditationDTO meditation;
    ResurrectionDTO resurrection;
    DeathDTO death;
    ChatListDTO list;
    ListItemsDTO items;
    ListBankDTO bank;
    ClanMessageDTO clan_msg;
    ClanFoundDTO clan_found;
    ClanAcceptDTO clan_accept;
    ClanLeaveDTO clan_leave;
    InventoryListDTO inventory_list;

    // TODO: REVISAR CONSTRUCTOR DEPENDIENDO DE COMO SE MANEJEN LAS ACTION EN EL FUTURO.
    // pueden llegar a no necesitar el ActionType dependiendo del contenido.
    //    explicit ActionDTO(ActionType action);

    explicit ActionDTO(const AttackDTO& attack);

    explicit ActionDTO(const DespawnDTO& despawn);

    explicit ActionDTO(const HealDTO& heal);

    explicit ActionDTO(const ChatMessageDTO& message);

    explicit ActionDTO(const MeditationDTO& meditation);

    explicit ActionDTO(const ResurrectionDTO& resurrection);

    explicit ActionDTO(const DeathDTO& death);

    explicit ActionDTO(const ChatListDTO& list);

    explicit ActionDTO(const ListItemsDTO& items);

    explicit ActionDTO(const ListBankDTO& bank);

    explicit ActionDTO(const ClanMessageDTO& clan_msg);

    explicit ActionDTO(const ClanAcceptDTO& clan_accept);

    explicit ActionDTO(const ClanFoundDTO& clan_found);

    explicit ActionDTO(const ClanLeaveDTO& clan_leave);

    explicit ActionDTO(const InventoryListDTO& inventory_list);

    // Modificarlo al agregar nuevas actions
    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};

#endif  // ACTION_DTO_H
