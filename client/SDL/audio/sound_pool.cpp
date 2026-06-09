#include "sound_pool.h"

#include <iostream>

#include <SDL.h>
#include <SDL2/SDL_mixer.h>


SoundPool::SoundPool():
        sdl(SDL_INIT_AUDIO), mixer(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096) {

    Mix_AllocateChannels(32);
}


void SoundPool::preload_sfx(const std::string& path) {
    if (not sounds.contains(path)) {
        try {
            sounds.emplace(path, SDL2pp::Chunk(path));

        } catch (const std::exception& e) {
            std::cerr << "[SoundPool] Error cargando SFX (" << path << "): " << e.what() << std::endl;
        }
    }
}


void SoundPool::play_sfx(const std::string& path, const int volume) {
    preload_sfx(path);
    if (sounds.contains(path)) {
        SDL2pp::Chunk& chunk = sounds.at(path);
        chunk.SetVolume(volume);
        Mix_PlayChannel(-1, chunk.Get(), 0);
    }
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
