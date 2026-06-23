#ifndef WORLD_H
#define WORLD_H

#include <map>
#include <memory>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "audio/audio_manager.h"
#include "common/dto/snapshot/actions/action.h"
#include "common/dto/snapshot/map/client_map_data.h"
#include "sprite_creation/sprite_creator.h"
#include "sprite_creation/texture_pool.h"
#include "sprites/fixed/effect_sprite.h"
#include "sprites/fixed/fixed_sprite.h"
#include "sprites/moving/enemy_sprite.h"
#include "sprites/moving/moving_sprite.h"
#include "sprites/moving/player_sprite.h"

#include "camera.h"

#define CELL_ITEMS_SIZE 4

struct CellSprites {
    std::array<std::shared_ptr<FixedSprite>, CELL_ITEMS_SIZE> sprites;
};

class World {
private:
    SpriteCreator& sprite_creator;
    AudioManager& audio_manager;

    SDL2pp::Rect world_view;
    std::string player_name;
    std::map<std::string, std::shared_ptr<PlayerSprite>> players;
    std::map<uint16_t, std::shared_ptr<EnemySprite>> creatures;
    std::map<std::pair<uint16_t, uint16_t>, std::pair<std::shared_ptr<FixedSprite>, LootType>> loot;

    std::vector<std::vector<CellSprites>> map_fixed_items;
    std::set<std::shared_ptr<MovingSprite>> map_entities;
    std::set<std::shared_ptr<EffectSprite>> effects;

    // Intancio para evitar crearlo constantemente antes de renderizar
    std::vector<std::shared_ptr<WorldSprite>> viewed_items;
    std::vector<std::shared_ptr<EffectSprite>> viewed_effects;

    void init_assets(const ClientMapDataDTO& map_data);

    void store_category_pointers(const std::vector<AssetInfoDTO>& assets, SpriteCategory category,
                                 int arr_index, const std::function<bool(SDL2pp::Point cell)>& condition);

    static bool cmp_by_y_coord(const std::shared_ptr<WorldSprite>& a, const std::shared_ptr<WorldSprite>& b);

    void add_new_creature(const CreatureInfoDTO& info);

    void add_new_loot(const LootInfoDTO& info, const std::pair<uint16_t, uint16_t>& place);

    void update_top_loot(const LootInfoDTO& info, const std::pair<uint16_t, uint16_t>& place);

    void erase_dead_creatures(const std::vector<CreatureInfoDTO>& creatures_information);

    void erase_taken_loot(const std::vector<LootInfoDTO>& loot_information);

    void handle_attack(const AttackDTO& attack);

    void play_event(const SoundEvent& event, const SDL2pp::Point& source);

    template <typename SpritePtrSrc, typename SpritePtrDst>
    auto filter_viewed_sprites(const Camera& camera, const std::set<std::shared_ptr<SpritePtrSrc>>& sprites,
                               std::vector<std::shared_ptr<SpritePtrDst>>& dest_vector) {
        auto is_visible = [&camera](const auto& item) {
            return item->intersects(camera.get_view(), camera.get_view().GetTopLeft());
        };

        std::ranges::copy_if(sprites, std::back_inserter(dest_vector), is_visible);
    }

public:
    World(SpriteCreator& sprite_creator, const ClientMapDataDTO& map_data, const std::string& player_name,
          AudioManager& audio_manager);

    void update_players(const std::vector<PlayerInfoDTO>& players_information);

    void update_creatures(const std::vector<CreatureInfoDTO>& creatures_information);

    void update_loot(const std::vector<LootInfoDTO>& loot_information);

    void erase_finished_effects();

    void handle_actions(const std::vector<ActionDTO>& actions);

    void update_visuals() const;

    void render_in_z_order(const Camera& camera, int iteration);

    PlayerSprite& get_client_player();

    SDL2pp::Rect& get_world_size();

    void add_new_player(const PlayerInfoDTO& info, bool is_client_player = false);
};


#endif  // WORLD_H
