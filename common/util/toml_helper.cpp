#include "toml_helper.h"

TomlHelper::TomlHelper() {}

SpriteCategory TomlHelper::get_sprite_category(const std::string& category) {
    const std::map<std::string, SpriteCategory> map = {
            {"head", SpriteCategory::HEAD},         {"body", SpriteCategory::BODY},
            {"tiles", SpriteCategory::TILE},        {"colliders", SpriteCategory::COLLIDER},
            {"npcs", SpriteCategory::NPC},          {"goblin", SpriteCategory::GOBLIN},
            {"skeleton", SpriteCategory::SKELETON}, {"zombie", SpriteCategory::ZOMBIE},
            {"spider", SpriteCategory::SPIDER},     {"orc", SpriteCategory::ORC},
            {"golem", SpriteCategory::GOLEM},       {"loot", SpriteCategory::LOOT}};

    return map.at(category);
}

// No se completa con la totalidad de opciones porque solo lo usa el editor
std::string TomlHelper::get_category_name(const SpriteCategory category) {
    const std::map<SpriteCategory, std::string> map = {
        {SpriteCategory::TILE, "tiles"},
        {SpriteCategory::COLLIDER, "colliders"},
        {SpriteCategory::NPC, "npcs"},
    };
    return map.at(category);
}
