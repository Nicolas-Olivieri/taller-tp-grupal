#ifndef AUDIO_PARSER_H
#define AUDIO_PARSER_H

#include <algorithm>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include <toml.hpp>


struct SoundConfig {
    std::vector<std::string> paths;
    uint8_t volume;  // Rango de 0 a 128
};


enum class MusicTrack { LOGIN, CREATOR, FOREST };

struct MusicConfig {
    std::map<MusicTrack, SoundConfig> configs;
};


enum class SoundEvent {
    DEATH,
    DESPAWN,
    FOOTSTEP,
    HEAL,
    MEDITATION,
    RESURRECTION,
    SPAWN,
    FISTS_ATTACK,
    SWORD_ATTACK,
    AXE_ATTACK,
    HAMMER_ATTACK,
    MAGIC_ARROW_SPELL,
    HEAL_SPELL,
    MISSILE_SPELL,
    EXPLOSION_SPELL,
    BOW_ATTACK,
};

struct SFXConfig {
    std::map<SoundEvent, SoundConfig> configs;
};


template <>
struct toml::from<MusicConfig> {
    static MusicConfig from_toml(const toml::value& v) {
        // TODO: Considerar moverlo a TomlHelper
        static const std::unordered_map<std::string, MusicTrack> string_to_music_track({
                {"login", MusicTrack::LOGIN},
                {"creator", MusicTrack::CREATOR},
                {"forest", MusicTrack::FOREST},
        });

        MusicConfig config;
        for (const auto& [key, value]: v.as_table()) {
            MusicTrack music_track = string_to_music_track.at(key);

            auto raw_paths = toml::find<std::vector<std::string>>(value, "paths");
            const int volume = toml::find<int>(value, "volume");

            std::vector<std::string> full_paths;
            full_paths.reserve(raw_paths.size());
            std::ranges::transform(raw_paths, std::back_inserter(full_paths),
                                   [](const std::string& path) { return std::string(DATA_PATH) + path; });

            config.configs[music_track] = SoundConfig(full_paths, volume);
        }

        return config;
    }
};


template <>
struct toml::from<SFXConfig> {
    static SFXConfig from_toml(const toml::value& v) {
        // TODO: Considerar moverlo a TomlHelper
        static const std::unordered_map<std::string, SoundEvent> string_to_sound_event({
                {"death", SoundEvent::DEATH},
                {"despawn", SoundEvent::DESPAWN},
                {"footstep", SoundEvent::FOOTSTEP},
                {"heal", SoundEvent::HEAL},
                {"meditation", SoundEvent::MEDITATION},
                {"resurrection", SoundEvent::RESURRECTION},
                {"spawn", SoundEvent::SPAWN},
                {"fists_attack", SoundEvent::FISTS_ATTACK},
                {"sword_attack", SoundEvent::SWORD_ATTACK},
                {"axe_attack", SoundEvent::AXE_ATTACK},
                {"hammer_attack", SoundEvent::HAMMER_ATTACK},
                {"magic_arrow_spell", SoundEvent::MAGIC_ARROW_SPELL},
                {"heal_spell", SoundEvent::HEAL_SPELL},
                {"missile_spell", SoundEvent::MISSILE_SPELL},
                {"explosion_spell", SoundEvent::EXPLOSION_SPELL},
                {"bow_attack", SoundEvent::BOW_ATTACK},
                // TODO: Agregar más sonidos...
        });

        SFXConfig config;
        for (const auto& [key, value]: v.as_table()) {
            SoundEvent event_type = string_to_sound_event.at(key);

            auto raw_paths = toml::find<std::vector<std::string>>(value, "paths");
            const int volume = toml::find<int>(value, "volume");

            std::vector<std::string> full_paths;
            full_paths.reserve(raw_paths.size());
            std::ranges::transform(raw_paths, std::back_inserter(full_paths),
                                   [](const std::string& path) { return std::string(DATA_PATH) + path; });

            config.configs[event_type] = SoundConfig(full_paths, volume);
        }

        return config;
    }
};


#endif  // AUDIO_PARSER_H
