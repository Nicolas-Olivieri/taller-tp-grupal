#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <memory>
#include <string>
#include <vector>

#include "SDL2pp/Renderer.hh"
#include "SDL2pp/Texture.hh"
#include "client/SDL/fonts/font_manager.h"
#include "client/SDL/ui/help_box_ui.h"
#include "common/dto/snapshot/actions/action.h"
#include "common/dto/snapshot/info/playerinfo.h"
#include "ui/chat_box_ui.h"
#include "ui/inventory_ui.h"

class UserInterface {
private:
    SDL2pp::Renderer& renderer;
    const UserInterfaceData& config;

    SpriteCreator sprite_creator;

    FontManager& font_manager;

    std::string& player_name;

    InventoryUI inventory_ui;
    ChatBoxUI chat_ui;
    InterfaceSprite game_border_ui;
    HelpBoxUi help_ui;

public:
    UserInterface(SDL2pp::Renderer& renderer, std::string& player_name, FontManager& font_manager);

    void render(const std::string& input, bool is_chat_active);

    // METODOS DEL CHAT ::::::
    void update_chat(const std::vector<ActionDTO>& actions);

    void chat_scroll_up();
    void chat_scroll_down();
    void chat_scroll_to_bottom();
    bool is_over_chat(int x, int y);


    // METODOS DEL INVENTARIO ::::::
    void update_player_state(const std::vector<PlayerInfoDTO>& players_information);

    std::optional<uint8_t> get_item_in_inventory_slot(int slot_index) const;
    std::optional<uint8_t> get_item_in_equipment_slot(int slot_index) const;
    int get_inventory_slot_at(int x, int y) const;
    int get_equipment_slot_at(int x, int y) const;

    void bind_item(int slot_index);
    std::optional<uint8_t> get_bound_item_id() const;
    void clear_bound_item();

    // METODOS DE AYUDA ::::::
    void toggle_help(const HelpPage& page);
    void render_help();
};


#endif  // USER_INTERFACE_H
