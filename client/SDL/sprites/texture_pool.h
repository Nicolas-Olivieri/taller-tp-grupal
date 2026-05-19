#ifndef TEXTURE_POOL_H
#define TEXTURE_POOL_H

#include <map>
#include <string>

#include "SDL2pp/SDL2pp.hh"


class TexturePool {
private:
    SDL2pp::Renderer& renderer;
    std::map<std::string, std::map<uint8_t, SDL2pp::Texture>> textures;

public:
    explicit TexturePool(SDL2pp::Renderer& renderer);

    SDL2pp::Texture& get_sprite_texture(const std::string& category_id,
                                        uint8_t sub_id);
};


#endif  // TEXTURE_POOL_H
