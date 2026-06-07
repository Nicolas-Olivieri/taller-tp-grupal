#ifndef ANIMATION_PARSER_H
#define ANIMATION_PARSER_H

#include <map>
#include <string>
#include <utility>
#include <vector>

#include <toml.hpp>

#include "client/SDL/sprites/animation.h"
#include "client/client_constants.h"
#include "common/direction.h"
#include "common/util/toml_helper.h"

template <typename keyType>
struct AnimationData {
    std::map<SpriteCategory, std::map<keyType, Animation>> animations;
};

struct AnimationTypesData {
    std::map<SpriteCategory, std::map<Direction, Animation>> walking_animations;
    std::map<SpriteCategory, std::map<uint8_t, Animation>> item_animations;
};

template <typename keyType>
struct toml::from<AnimationData<keyType>> {
    static AnimationData<keyType> from_toml(const value& raw) {
        AnimationData<keyType> data;
        const auto& file = raw.as_table();

        for (const auto& [category, raw_cat_anim]: file) {
            SpriteCategory parsed_cat = TomlHelper::get_sprite_category(category);
            std::map<keyType, Animation> category_animations;

            const auto& animations_data = raw_cat_anim.as_table();

            for (const auto& [animation_id, frames]: animations_data) {
                auto dir = static_cast<keyType>(std::stoi(animation_id));
                auto vec = toml::get<std::vector<int>>(frames);
                Animation anim(vec[0], vec[1], vec[2], vec[3], vec[4], vec[5]);

                category_animations.insert({{dir, std::move(anim)}});
            }

            data.animations.insert({{parsed_cat, std::move(category_animations)}});
        }
        return data;
    }
};


template <>
struct toml::from<AnimationTypesData> {
    static AnimationTypesData from_toml(const value& v) {
        AnimationTypesData data;
        const auto& table = v.as_table();

        if (table.contains("walking")) {
            auto walking_data = toml::get<AnimationData<Direction>>(table.at("walking"));
            data.walking_animations = std::move(walking_data.animations);
        }

        if (table.contains("items")) {
            auto item_data = toml::get<AnimationData<uint8_t>>(table.at("items"));
            data.item_animations = std::move(item_data.animations);
        }

        return data;
    }
};


#endif  // ANIMATION_PARSER_H
