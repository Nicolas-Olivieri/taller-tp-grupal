#include "meditation.h"

#include "common/protocol/serializer.h"


MeditationDTO::MeditationDTO(): player_meditating("") {}


MeditationDTO::MeditationDTO(const std::string& player_meditating): player_meditating(player_meditating) {}


size_t MeditationDTO::message_size() const { return sizeof(uint16_t) + player_meditating.size(); }


void MeditationDTO::accept(Serializer& serializer) const { serializer.serialize(*this); }
