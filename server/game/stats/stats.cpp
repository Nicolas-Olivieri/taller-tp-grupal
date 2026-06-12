#include "stats.h"

#include "server/util/calculator.h"

Stats::Stats(uint8_t archetype_id, uint8_t race_id, uint32_t current_xp_amount, uint8_t xp_level):
        archetype_id(archetype_id),
        race_id(race_id),
        experience(current_xp_amount, xp_level),
        agility(Calculator::calculate_averagable_stat(race().agility, archetype().agility)),
        constitution(Calculator::calculate_scalable_stat(race().constitution, xp_level,
                                                         archetype().constitution_multiplier)),
        intelligence(Calculator::calculate_scalable_stat(race().intelligence, xp_level,
                                                         archetype().intelligence_multiplier)),
        strength(Calculator::calculate_scalable_stat(race().strength, xp_level,
                                                     archetype().strength_multiplier)),
        health(race().recovery_factor, archetype().health_factor, race().health_factor, constitution,
               xp_level),
        mana(race().recovery_factor, archetype().mana_factor, race().mana_factor,
             archetype().meditation_factor, intelligence, xp_level) {}

Stats::Stats(uint8_t race_id, uint8_t variation_id, uint8_t xp_level):
        archetype_id(variation_id),
        race_id(race_id),
        experience(0, xp_level),
        agility(Calculator::calculate_averagable_stat(creature().agility, variation().agility)),
        constitution(Calculator::calculate_scalable_stat(creature().constitution, xp_level,
                                                         variation().multiplier)),
        intelligence(Calculator::calculate_scalable_stat(creature().intelligence, xp_level,
                                                         variation().multiplier)),
        strength(Calculator::calculate_scalable_stat(creature().strength, xp_level, variation().multiplier)),
        health(creature().recovery_factor, variation().factor, creature().health_factor, constitution,
               xp_level),
        mana(creature().recovery_factor, variation().factor, creature().mana_factor, 0, intelligence,
             xp_level) {}

const ArchetypeData& Stats::archetype() const { return GameConfig::get().get_archetype(archetype_id); }

const RaceData& Stats::race() const { return GameConfig::get().get_race(race_id); }

const RaceData& Stats::creature() const { return GameConfig::get().get_creature(race_id); }

const VariationData& Stats::variation() const { return GameConfig::get().get_variation(archetype_id); }

// TODO: para más robustez del sistema, estaría bueno agregar algo para que las creatures no llamen a este
// método
void Stats::upgrade() {
    uint8_t new_level = experience.get_level();

    constitution = Calculator::calculate_scalable_stat(race().constitution, new_level,
                                                       archetype().constitution_multiplier);
    intelligence = Calculator::calculate_scalable_stat(race().intelligence, new_level,
                                                       archetype().intelligence_multiplier);
    strength =
            Calculator::calculate_scalable_stat(race().strength, new_level, archetype().strength_multiplier);

    health.update_max(new_level, constitution);
    mana.update_max(new_level, intelligence);
}
