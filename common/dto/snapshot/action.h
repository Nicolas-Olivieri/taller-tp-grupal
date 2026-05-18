#ifndef ACTION_H
#define ACTION_H

#include <cstdint>

#include "common/dto/message.h"
#include "common/dto/snapshot/appearance.h"

enum class ActionType : uint8_t { APPEARANCE };

struct ActionDTO: public ProtocolMessageDTO {
    ActionType action;
    AppearanceDTO appearance;

    // TODO: debería agregarse un constructor por cada acción definida, donde el
    // ActionType se define correspondientemente
    explicit ActionDTO(const AppearanceDTO& appearance);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};

#endif  // ACTION_H
