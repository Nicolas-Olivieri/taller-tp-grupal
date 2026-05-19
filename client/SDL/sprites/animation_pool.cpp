#include "animation_pool.h"

#include <string>
#include <utility>

#include <toml.hpp>

#include "../../TOML/animation_parser.h"

AnimationPool::AnimationPool() {
    auto root = toml::parse(DATA_PATH "/animations.toml");
    auto [animations_data] = toml::find<AnimationData>(root, "animations");

    this->animations = std::move(animations_data);
}

std::map<Direction, Animation>& AnimationPool::get_animation(const std::string& category) {
    return animations.at(category);
}
