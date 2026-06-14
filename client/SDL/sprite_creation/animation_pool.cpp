#include "animation_pool.h"

#include <string>
#include <utility>

#include <toml.hpp>

#include "../../TOML/animation_parser.h"

AnimationPool::AnimationPool() {
    auto root = toml::parse(CONFIG_PATH "/animations.toml");
    auto data = toml::get<AnimationTypesData>(root);

    this->walking_animations = std::move(data.walking_animations);
    this->item_animations = std::move(data.item_animations);
}


std::map<Direction, Animation>& AnimationPool::get_walking_animations(const SpriteCategory category) {
    return walking_animations.at(category);
}

Animation& AnimationPool::get_item_animation(const SpriteCategory category, const uint8_t asset_id) {
    return item_animations.at(category).at(asset_id);
}
