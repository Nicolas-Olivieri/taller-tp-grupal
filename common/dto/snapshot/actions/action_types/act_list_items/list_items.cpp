#include "list_items.h"

#include "common/protocol/serializer.h"


ListItemsDTO::ListItemsDTO(const MessageType& type, const std::map<uint8_t, uint16_t>& items,
                           const std::string& receiver):
        type(type), items(items), receiver(receiver) {}


size_t ListItemsDTO::message_size() const {
    // Tamaño total = tamaño del receiver (uint16_t + string) + tamaño del mapa (uint16_t para la cantidad de
    // elementos + (tamaño de la key + tamaño del value) * cantidad)
    return sizeof(MessageType) + sizeof(uint16_t) + receiver.size() + sizeof(uint16_t) +
           ((sizeof(uint8_t) + sizeof(uint16_t)) * items.size());
}


void ListItemsDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
