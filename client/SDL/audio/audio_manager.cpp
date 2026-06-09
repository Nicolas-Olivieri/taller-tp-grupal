#include "audio_manager.h"

#include <string>

#define AUDIO_PATH "/client/audio.toml"


AudioManager::AudioManager(): sound_pool(SoundPool()) { load_audio_config(); }


void AudioManager::load_audio_config() {
    auto root = toml::parse(std::string(CONFIG_PATH) + std::string(AUDIO_PATH));
    if (root.contains("music")) {
        auto [configs] = toml::find<MusicConfig>(root, "music");
        for (const auto& [track, config]: configs) {
            track_to_paths[track] = config;
        }
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
