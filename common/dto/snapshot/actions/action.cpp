#include "action.h"

#include "common/protocol/serializer.h"

ActionDTO::ActionDTO(const AppearanceDTO& appearance):
        action(ActionType::APPEARANCE), appearance(appearance) {}

// TODO: este método debería cambiar su comportamiento dependiendo del
// ActionType, considerar opciones de implementación
size_t ActionDTO::message_size() const { return sizeof(action) + appearance.message_size(); }

void ActionDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
