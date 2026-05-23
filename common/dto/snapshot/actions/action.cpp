#include "action.h"

#include "common/protocol/serializer.h"

ActionDTO::ActionDTO(ActionType action): action(action) {}

// TODO: este método debería cambiar su comportamiento dependiendo del
// ActionType, considerar opciones de implementación
size_t ActionDTO::message_size() const { return sizeof(action); }

void ActionDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
