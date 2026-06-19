#ifndef INVENTORY_UI_H
#define INVENTORY_UI_H

#include <string>
#include <utility>
#include <vector>

#include "client/SDL/sprite_creation/sprite_creator.h"
#include "client/SDL/sprites/ui/progress_bar_sprite.h"
#include "client/config/client_data.h"

class InventoryUI {
    friend class UserInterface;

private:
    SpriteCreator& creator;
    const UserInterfaceData& ui_config;

    InterfaceSprite ui;

    std::vector<ProgressBarSprite> bars;
    std::vector<HudSprite> inventory;
    std::vector<HudSprite> equipment;
    std::vector<TextSprite> equipment_state;

    std::optional<InterfaceSprite> founder_crown;
    std::optional<int> bounded_slot;

    SDL_Color white;

    TextSprite player_name;
    TextSprite clan_name;
    TextSprite inventory_label;
    TextSprite statistics_label;

    TextSprite xp_level;
    TextSprite safe_gold;
    TextSprite excess_gold;

    int get_slot_at(const std::vector<SDL2pp::Rect>& _slots, int x, int y) const;

    std::optional<uint8_t> get_item_in_slot(const std::vector<HudSprite>& _slots, int slot_index) const;

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
