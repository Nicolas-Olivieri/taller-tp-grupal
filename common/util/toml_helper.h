#ifndef TOML_HELPER_H
#define TOML_HELPER_H
#include <map>
#include <string>

#include "common/util/constants.h"


class TomlHelper {
public:
    TomlHelper();

    static SpriteCategory get_sprite_category(const std::string& category);

    static std::string get_category_name(SpriteCategory category);
};


#endif  // TOML_HELPER_H
