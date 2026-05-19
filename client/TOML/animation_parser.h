#ifndef ANIMATION_PARSER_H
#define ANIMATION_PARSER_H

#include <map>
#include <string>
#include <utility>
#include <vector>

#include <toml.hpp>

#include "client/SDL/sprites/animation.h"
#include "common/direction.h"

struct AnimationData {
    std::map<std::string, std::map<Direction, Animation>> animations;
};

template <>
struct toml::from<AnimationData> {
    static AnimationData from_toml(const value& raw) {
        AnimationData data;
        const auto& file = raw.as_table();

        for (const auto& [category, raw_cat_anim]: file) {
            std::map<Direction, Animation> category_animations;

            const auto& animations_data = raw_cat_anim.as_table();

            for (const auto& [direction, frames]: animations_data) {
                auto dir = static_cast<Direction>(std::stoi(direction));
                auto vec = toml::get<std::vector<int>>(frames);
                Animation anim(vec[0], vec[1], vec[2], vec[3], vec[4]);

                category_animations.insert({{dir, std::move(anim)}});
            }

            data.animations.insert({{category, std::move(category_animations)}});
        }
        return data;
    }
};


#endif  // ANIMATION_PARSER_H
