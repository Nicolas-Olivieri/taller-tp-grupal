#include "action.h"

#include "common/protocol/serializer.h"

// TODO: este método debería cambiar su comportamiento dependiendo del
// ActionType, considerar opciones de implementación
size_t ActionDTO::message_size() const { return sizeof(action); }

void ActionDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }

ActionDTO::ActionDTO(const DespawnDTO& despawn): action(ActionType::DESPAWN), despawn(despawn) {}
