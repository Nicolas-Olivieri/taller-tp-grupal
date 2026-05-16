#include "texture_pool.h"

#define BODY_OFFX 256
#define BODY_OFFY 0
#define BODY_W 27
#define BODY_H 47

#define HEAD_OFFX 162
#define HEAD_OFFY 256
#define HEAD_W 27
#define HEAD_H 64


TexturePool::TexturePool(SDL2pp::Renderer& renderer) :
    renderer(renderer) {

    // Se carga un vector/map a partir de TOML de todos los nombres de archivos que son para surfaces/textures
    // Se hace un for que itera el vector/map y guarda id -> Texture/Surface(DATA_PATH '/nombre_actual.png')
    // Se repite lo mismo para el mapa de rects

    textures.try_emplace(1, renderer, DATA_PATH "/heads.png");
    textures.try_emplace(2, renderer, DATA_PATH "/bodies.png");

    base_rects.insert({{1, SDL2pp::Rect(HEAD_OFFX, HEAD_OFFY, HEAD_W, HEAD_H)},
                            {2, SDL2pp::Rect(BODY_OFFX, BODY_OFFY, BODY_W, BODY_H)}});

}


SDL2pp::Texture& TexturePool::get_sprite_texture(const uint8_t id) {
    return textures.at(id);
}

SDL2pp::Rect TexturePool::get_sprite_rect(const uint8_t id) const {
    return base_rects.at(id);
}