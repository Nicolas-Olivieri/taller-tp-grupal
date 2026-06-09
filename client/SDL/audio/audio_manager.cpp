#include "audio_manager.h"

#include <string>

#define AUDIO_PATH "/client/audio.toml"


AudioManager::AudioManager(): sound_pool(SoundPool()) { load_audio_config(); }


void AudioManager::load_audio_config() {
    const auto root = toml::parse(std::string(CONFIG_PATH) + std::string(AUDIO_PATH));
    load_music_config(root);
    load_sfx_config(root);
}


void AudioManager::load_music_config(toml::basic_value<toml::type_config>::array_type::const_reference root) {
    if (root.contains("music")) {
        auto [configs] = toml::find<MusicConfig>(root, "music");
        for (const auto& [track, config]: configs) {
            track_to_paths[track] = config;
            track_index[track] = 0;
        }
    }
}


void AudioManager::load_sfx_config(toml::basic_value<toml::type_config>::array_type::const_reference root) {
    if (root.contains("sfx")) {
        auto [configs] = toml::find<SFXConfig>(root, "sfx");
        for (const auto& [event, config]: configs) {
            event_to_paths[event] = config;
            event_index[event] = 0;
        }
    }
}


void AudioManager::play_event(const SoundEvent& event) {
    if (event_to_paths.contains(event)) {
        const auto& [paths, volume] = event_to_paths.at(event);
        size_t& current_index = event_index[event];
        sound_pool.play_sfx(paths[current_index], volume);
        current_index = (current_index + 1) % paths.size();
    }
}


void AudioManager::play_music(const MusicTrack& track) {
    if (track_to_paths.contains(track)) {
        const auto& [paths, volume] = track_to_paths.at(track);
        size_t& current_index = track_index[track];
        sound_pool.play_music(paths[current_index], volume);
        current_index = (current_index + 1) % paths.size();
    }
}
