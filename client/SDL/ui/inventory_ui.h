#ifndef INVENTORY_UI_H
#define INVENTORY_UI_H

#include <string>
#include <utility>
#include <vector>

#include "client/SDL/sprite_creation/sprite_creator.h"
#include "client/SDL/sprites/ui/progress_bar_sprite.h"

class InventoryUI {
    friend class UserInterface;

private:
    SpriteCreator& creator;

    InterfaceSprite ui;

    std::vector<ProgressBarSprite> bars;
    std::vector<HudSprite> inventory;
    std::vector<HudSprite> equipment;

    std::optional<int> bounded_slot;

    // TODO refactorizar con toml
    const std::vector<SDL2pp::Rect> inventory_slots = {
            {806, 220, 34, 34}, {874, 220, 34, 34}, {942, 220, 34, 34},
            {806, 284, 34, 34}, {874, 284, 34, 34}, {942, 284, 34, 34},
            {806, 348, 34, 34}, {874, 348, 34, 34}, {942, 348, 34, 34}};

    const std::vector<SDL2pp::Rect> equipment_slots = {{780, 447, 34, 34},   // Espada
                                                       {842, 447, 34, 34},   // Escudo
                                                       {906, 447, 34, 34},   // Casco
                                                       {968, 447, 34, 34}};  // Armadura

    SDL_Color white = {255, 255, 255, 255};
    SDL_Color yellow = {235, 224, 70, 255};

    SDL2pp::Rect clan_rect = {770, 100, 240, 20};
    SDL2pp::Rect username_rect = {770, 55, 240, 35};
    SDL2pp::Rect inventory_rect = {770, 157, 240, 35};
    SDL2pp::Rect stats_rect = {770, 519, 240, 35};
    SDL2pp::Rect health_rect = {791, 599, 216, 15};
    SDL2pp::Rect mana_rect = {791, 627, 216, 15};
    SDL2pp::Rect xp_rect = {837, 657, 171, 15};
    SDL2pp::Rect safe_gold_rect = {791, 562, 90, 16};
    SDL2pp::Rect excess_gold_rect = {918, 562, 90, 16};
    SDL2pp::Rect xp_level_rect = {787, 657, 37, 16};

    TextSprite player_name;
    TextSprite clan_name;
    TextSprite inventory_label;
    TextSprite statistics_label;

    TextSprite xp_level;
    TextSprite safe_gold;
    TextSprite excess_gold;

    int get_slot_at(const std::vector<SDL2pp::Rect>& slots, int x, int y) const;

    std::optional<uint8_t> get_item_in_slot(const std::vector<HudSprite>& slots, int slot_index) const;

public:
    InventoryUI(SpriteCreator& sprite_creator, const std::string& username);

    void init_elements();

    void update_player_state(const std::vector<PlayerInfoDTO>& players_information);

    void render();

    void bind_item(int slot_index);
    void clear_bound_item();
    std::optional<uint8_t> get_bound_item_id() const;
};


#endif  // INVENTORY_UI_H
