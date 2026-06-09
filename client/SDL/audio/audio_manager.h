#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <map>

#include "client/TOML/audio_parser.h"

#include "sound_pool.h"


class AudioManager {
private:
    SoundPool sound_pool;

    std::map<MusicTrack, SoundConfig> track_to_paths;
    std::map<MusicTrack, size_t> track_index;

public:
    AudioManager();

    void play_music(const MusicTrack& track);

private:
    void load_audio_config();
};


#endif  // AUDIO_MANAGER_H
