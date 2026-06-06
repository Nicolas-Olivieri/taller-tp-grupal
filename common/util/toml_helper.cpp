#include "toml_helper.h"

TomlHelper::TomlHelper() {}

SpriteCategory TomlHelper::get_sprite_category(const std::string& category) {
    const std::map<std::string, SpriteCategory> map = {{"head", SpriteCategory::HEAD},
                                                       {"body", SpriteCategory::BODY},
                                                       {"tiles", SpriteCategory::TILE},
                                                       {"colliders", SpriteCategory::COLLIDER},
                                                       {"npcs", SpriteCategory::NPC}};

    return map.at(category);
}
