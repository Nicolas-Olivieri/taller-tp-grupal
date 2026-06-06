#ifndef TOML_HELPER_H
#define TOML_HELPER_H
#include <map>
#include <string>

#include "client/client_constants.h"


class TomlHelper {
public:
    TomlHelper();

    static SpriteCategory get_sprite_category(const std::string& category);
};


#endif  // TOML_HELPER_H
