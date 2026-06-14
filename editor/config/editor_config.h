#ifndef EDITOR_CONFIG_H
#define EDITOR_CONFIG_H

#include <vector>
#include <string>
#include <unordered_map>
#include "components.h"
#include "toml.hpp"

class EditorConfig {
private:
    std::unordered_map<std::string, std::vector<AssetData>> category_to_assets_data;

    EditorConfig();

public:
    static EditorConfig& get();

    std::vector<AssetData> get_assets_data(const std::string& category);
};


#endif  // EDITOR_CONFIG_H
