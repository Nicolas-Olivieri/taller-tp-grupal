#ifndef ANIMATION_PARSER_H
#define ANIMATION_PARSER_H

#include <map>
#include <string>
#include <utility>
#include <vector>

#include <toml.hpp>

#include "client/SDL/sprites/animation.h"
#include "common/direction.h"
#include "common/util/toml_helper.h"

template <typename keyType>
struct AnimationData {
    std::map<SpriteCategory, std::map<keyType, Animation>> data;
};

struct AnimationTypesData {
    std::map<SpriteCategory, std::map<Direction, Animation>> walking_animations;
    std::map<SpriteCategory, std::map<uint8_t, Animation>> item_animations;
};

template <>
struct toml::from<AnimationData<Direction>> {
    static AnimationData<Direction> from_toml(const value& raw) {
        AnimationData<Direction> walking_animations;
        const auto& file = raw.as_table();

        for (const auto& [category, raw_cat_anim]: file) {
            SpriteCategory parsed_cat = TomlHelper::get_sprite_category(category);
            std::map<Direction, Animation> category_animations;

            const auto& animations_data = raw_cat_anim.as_table();

            for (const auto& [animation_id, frames]: animations_data) {
                auto id = static_cast<Direction>(std::stoi(animation_id));
                auto vec = toml::get<std::vector<int>>(frames);
                Animation anim(vec[0], vec[1], vec[2], vec[3], vec[4], vec[5]);
                category_animations.insert({{id, anim}});
            }

            walking_animations.data.insert({{parsed_cat, std::move(category_animations)}});
        }
        return walking_animations;
    }
};

template <>
struct toml::from<AnimationData<uint8_t>> {
    static AnimationData<uint8_t> from_toml(const value& raw) {
        AnimationData<uint8_t> item_animations;
        const auto& categories = raw.as_table();

        for (const auto& [category, raw_cat_anim]: categories) {
            std::map<uint8_t, Animation> category_animations;
            SpriteCategory parsed_cat = TomlHelper::get_sprite_category(category);
            const auto& animations = raw_cat_anim.as_array();

            for (auto& animation : animations) {
                auto vec = toml::get<std::vector<int>>(animation.at("data"));
                Animation anim(vec[0], vec[1], vec[2], vec[3], vec[4], vec[5]);

                if (animation.contains("id")) {
                    auto id = static_cast<uint8_t>(toml::get<int>(animation.at("id")));
                    category_animations.insert({{id, anim}});

                } else if (animation.contains("id_range")) {
                    auto range = toml::get<std::vector<int>>(animation.at("id_range"));
                    const uint8_t start = static_cast<uint8_t>(range[0]);
                    const uint8_t end   = static_cast<uint8_t>(range[1]);
                    for (uint8_t id = start; id <= end; ++id) {
                        category_animations.insert({{id, anim}});
                    }
                }
            }
            item_animations.data.insert({{parsed_cat, std::move(category_animations)}});
        }

        return item_animations;
    }
};


template <>
struct toml::from<AnimationTypesData> {
    static AnimationTypesData from_toml(const value& v) {
        AnimationTypesData animations;
        const auto& table = v.as_table();

        if (table.contains("walking")) {
            auto walking_data = toml::get<AnimationData<Direction>>(table.at("walking"));
            animations.walking_animations = std::move(walking_data.data);
        }

        if (table.contains("items")) {
            auto item_data = toml::get<AnimationData<uint8_t>>(table.at("items"));
            animations.item_animations = std::move(item_data.data);
        }

        return animations;
    }
};


#endif  // ANIMATION_PARSER_H
