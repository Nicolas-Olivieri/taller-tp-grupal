#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <map>

#include "client/TOML/audio_parser.h"

#include "sound_pool.h"


class AudioManager {
private:
    SoundPool sound_pool;

    std::map<SoundEvent, SoundConfig> event_to_paths;
    std::map<SoundEvent, size_t> event_index;

    std::map<MusicTrack, SoundConfig> track_to_paths;
    std::map<MusicTrack, size_t> track_index;

public:
    AudioManager();

    void play_event(const SoundEvent& event);

    void play_music(const MusicTrack& track);

private:
    void load_audio_config();

    void load_music_config(toml::basic_value<toml::type_config>::array_type::const_reference root);

    void load_sfx_config(toml::basic_value<toml::type_config>::array_type::const_reference root);
};


#endif  // AUDIO_MANAGER_H
