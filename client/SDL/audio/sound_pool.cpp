#include "sound_pool.h"

#include <iostream>

#include <SDL.h>
#include <SDL2/SDL_mixer.h>


SoundPool::SoundPool():
        sdl(SDL_INIT_AUDIO), mixer(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096) {

    Mix_AllocateChannels(32);
}


void SoundPool::play_music(const std::string& path, const int volume) {
    if (current_music_path == path) {
        return;
    }

    try {
        current_music.emplace(path);
        current_music_path = path;
        Mix_VolumeMusic(volume);
        Mix_PlayMusic(current_music->Get(), -1);

    } catch (const std::exception& e) {
        std::cerr << "[SoundPool] Error cargando música (" << path << "): " << e.what() << '\n';
        current_music.reset();
        current_music_path.clear();
    }
}


void SoundPool::set_music_volume(const int volume) { Mix_VolumeMusic(volume); }
