#include "texture_pool.h"

#include <string>
#include <utility>
#include <vector>

#include <toml.hpp>

#include "client/client_constants.h"
#include "common/util/toml_helper.h"


TexturePool::TexturePool(SDL2pp::Renderer& renderer): renderer(renderer) {
    auto root = toml::parse(CONFIG_PATH "/texture_files.toml");
    auto files = toml::find(root, "file_ids");

    for (const auto& [category, ids]: files.as_table()) {
        auto values = toml::get<std::vector<int>>(ids);
        const int start = values[0];
        const int finish = values[1];

        std::map<uint8_t, SDL2pp::Texture> category_textures;
        for (int i = start; i <= finish; ++i) {
            try {
                std::string path = std::format("{}/{}/{}.png", DATA_PATH, category, i);
                SDL2pp::Texture texture(renderer, path);
                category_textures.insert({i, std::move(texture)});
            } catch (const SDL2pp::Exception& err) {
                // Skip si no existe ese índice, ideal cuando la variation de las creatures dependen de la
                // imagen
            }
        }

        SpriteCategory parsed_cat = TomlHelper::get_sprite_category(category);
        textures.insert({parsed_cat, std::move(category_textures)});
    }
}


SDL2pp::Texture& TexturePool::get_sprite_texture(const SpriteCategory category_id, const uint8_t sub_id) {
    return textures.at(category_id).at(sub_id);
}
