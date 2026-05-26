#include "action.h"

#include <stdexcept>

#include "common/protocol/serializer.h"

// TODO: este método debería cambiar su comportamiento dependiendo del
// ActionType, considerar opciones de implementación
size_t ActionDTO::message_size() const {
    const auto base = sizeof(action);

    switch (action) {
        case ActionType::DESPAWN:
            return base + despawn.message_size();
        default:
            throw std::runtime_error("ActionDTO Descubrió que tiene un tipo de acción desconocido al "
                                     "calcular su message_size");
    }
}

void ActionDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }

ActionDTO::ActionDTO(const DespawnDTO& despawn): action(ActionType::DESPAWN), despawn(despawn) {}
