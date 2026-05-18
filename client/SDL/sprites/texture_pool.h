#ifndef TEXTURE_POOL_H
#define TEXTURE_POOL_H

#include <map>

#include "SDL2pp/SDL2pp.hh"


class TexturePool {
private:
    SDL2pp::Renderer& renderer;
    std::map<uint8_t, SDL2pp::Texture> textures;

    // mapa de rects por id obtenido a partir de un toml
    std::map<uint8_t, SDL2pp::Rect> base_rects;

public:
    explicit TexturePool(SDL2pp::Renderer& renderer);

    SDL2pp::Texture& get_sprite_texture(uint8_t id);
    SDL2pp::Rect get_sprite_rect(uint8_t id) const;
};


#endif  // TEXTURE_POOL_H
