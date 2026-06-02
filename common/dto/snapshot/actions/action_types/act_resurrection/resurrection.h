#ifndef RESURRECTION_H
#define RESURRECTION_H


#include <string>

#include "common/dto/message.h"
#include "common/dto/snapshot/info/appearance.h"

struct ResurrectionDTO: public ProtocolMessageDTO {

    std::string player_resurrected;
    AppearanceDTO original_appearance;

    explicit ResurrectionDTO(const std::string& player_name, const AppearanceDTO& appearance);

    size_t message_size() const override;

    void accept(Serializer& serializer) const override;
};


#endif  // RESURRECTION_H
