#ifndef SNAPSHOTDTO_H
#define SNAPSHOTDTO_H

#include <vector>

#include "actions/action.h"
#include "common/dto/message.h"
#include "info/playerinfo.h"

struct SnapshotDTO: public ProtocolMessageDTO {
    std::vector<PlayerInfoDTO> players_information;
    // std::vector<CreatureInfdDTO> creatures_information; TODO: agregar esto Y
    // NO OLVIDAR MODIFICAR LOS MÉTODOS PARA TENERLO EN CUENTA
    std::vector<ActionDTO> actions;

    SnapshotDTO(const std::vector<PlayerInfoDTO>& players_information, const std::vector<ActionDTO>& actions);

    // Constructor dummy
    SnapshotDTO() = default;

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};

#endif  // SNAPSHOTDTO_H
