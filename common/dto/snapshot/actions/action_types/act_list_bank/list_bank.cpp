#include "list_bank.h"

#include "common/protocol/serializer.h"


ListBankDTO::ListBankDTO(): type(MessageType::SYSTEM), gold(0), items({}), receiver("") {}


ListBankDTO::ListBankDTO(const MessageType& type, const uint16_t gold,
                         const std::map<uint8_t, uint16_t>& items, const std::string& receiver):
        type(type), gold(gold), items(items), receiver(receiver) {}


size_t ListBankDTO::message_size() const {
    // Tamaño total = tamaño del receiver (uint16_t + string) + tamaño del mapa (uint16_t para la cantidad de
    // elementos + (tamaño de la key + tamaño del value) * cantidad) + cantidad de oro (uint16_t)
    return sizeof(MessageType) + sizeof(uint16_t) + receiver.size() + sizeof(uint16_t) +
           (sizeof(uint8_t) + sizeof(uint16_t)) * items.size() + sizeof(uint16_t);
}


void ListBankDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
