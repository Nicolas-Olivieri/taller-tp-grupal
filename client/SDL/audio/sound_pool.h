#ifndef SOUND_POOL_H
#define SOUND_POOL_H

#include <map>
#include <optional>
#include <string>

#include <SDL2pp/Chunk.hh>

#include "SDL2pp/Mixer.hh"
#include "SDL2pp/Music.hh"
#include "SDL2pp/SDL.hh"


class SoundPool {
private:
    SDL2pp::SDL sdl;
    SDL2pp::Mixer mixer;

    std::map<std::string, SDL2pp::Chunk> sounds;

    std::optional<SDL2pp::Music> current_music;
    std::string current_music_path;

public:
    SoundPool();

    void preload_sfx(const std::string& path);

    void play_sfx(const std::string& path, int volume = 32);

    void play_music(const std::string& path, int volume = 64);

    void set_music_volume(int volume);
};


#endif  // SOUND_POOL_H
