#include "texture_pool.h"

#include <string>
#include <utility>
#include <vector>

#include <toml.hpp>


TexturePool::TexturePool(SDL2pp::Renderer& renderer): renderer(renderer) {
    auto root = toml::parse(DATA_PATH "/texture_files.toml");
    auto files = toml::find(root, "file_ids");

    for (const auto& [category, ids]: files.as_table()) {
        auto values = toml::get<std::vector<int>>(ids);
        const int start = values[0];
        const int finish = values[1];

        std::map<uint8_t, SDL2pp::Texture> category_textures;
        for (int i = start; i <= finish; ++i) {
            std::string path = std::format("{}/{}/{}.png", DATA_PATH, category, i);
            SDL2pp::Texture texture(renderer, path);

            category_textures.insert({i, std::move(texture)});
        }

        textures.insert({category, std::move(category_textures)});
    }
}


SDL2pp::Texture& TexturePool::get_sprite_texture(const std::string& category_id, const uint8_t sub_id) {
    return textures.at(category_id).at(sub_id);
}
