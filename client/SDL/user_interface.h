#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H


#include <deque>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <SDL2pp/Font.hh>

#include "SDL2pp/Renderer.hh"
#include "SDL2pp/Texture.hh"
#include "common/dto/snapshot/actions/action.h"
#include "common/dto/snapshot/info/playerinfo.h"

struct BarValue {
    SDL2pp::Texture& texture;
    size_t current;
    size_t max;
};

struct InventorySlotData {
    uint8_t item_id;
    uint8_t amount;
};

class UserInterface {
private:
    SDL2pp::Renderer& renderer;
    SDL2pp::Font user_font;
    /* SDL2pp::Font clan_font; */
    SDL2pp::Font menu_title_font;
    SDL2pp::Font menu_font;
    SDL2pp::Font chat_font;
    SDL2pp::Font item_amount_font;
    // TODO convertirlo en un sprite
    SDL2pp::Texture ui_texture;
    std::string& player_name;

    std::deque<std::pair<std::string, SDL_Color>> chat_history;
    size_t first_visible_message = 0;

    std::vector<std::pair<SDL2pp::Rect, std::string>> field_values;
    std::vector<std::pair<SDL2pp::Rect, BarValue>> bar_values;

    SDL_Color yellow = {235, 224, 70, 255};
    SDL_Color grey = {255, 255, 255, 140};
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color green = {44, 230, 66, 140};
    SDL_Color red = {214, 30, 30, 255};
    SDL_Color light_blue = {44, 172, 230, 140};

    SDL2pp::Rect history_messages = {20, 35, 600, 147};
    SDL2pp::Rect input_box = {45, 190, 580, 25};

    SDL2pp::Rect username_rect = {770, 55, 240, 35};
    /* SDL2pp::Rect clan_name = {770, 100, 240, 20}; */
    SDL2pp::Rect inventory_rect = {770, 157, 240, 35};

    std::vector<InventorySlotData> current_inventory;
    const std::vector<SDL2pp::Rect> inventory_slots = {
            {806, 220, 34, 34}, {874, 220, 34, 34}, {942, 220, 34, 34},
            {806, 284, 34, 34}, {874, 284, 34, 34}, {942, 284, 34, 34},
            {806, 348, 34, 34}, {874, 348, 34, 34}, {942, 348, 34, 34}};

    std::optional<uint8_t> bound_item_id;
    std::optional<int> bound_slot_index;

    std::vector<uint8_t> current_equipment;
    const std::vector<SDL2pp::Rect> equipment_slots = {{780, 447, 34, 34},   // Espada
                                                       {842, 447, 34, 34},   // Escudo
                                                       {906, 447, 34, 34},   // Casco
                                                       {968, 447, 34, 34}};  // Armadura

    std::map<uint8_t, std::unique_ptr<SDL2pp::Texture>> item_textures;

    SDL2pp::Rect stats_rect = {770, 519, 240, 35};

    SDL2pp::Rect health_rect = {791, 599, 216, 15};
    SDL2pp::Texture health_texture;
    SDL2pp::Rect mana_rect = {791, 627, 216, 15};
    SDL2pp::Texture mana_texture;
    SDL2pp::Rect xp_rect = {837, 657, 171, 15};
    SDL2pp::Texture xp_texture;

    SDL2pp::Rect safe_gold_rect = {791, 562, 90, 16};
    SDL2pp::Rect excess_gold_rect = {918, 562, 90, 16};
    SDL2pp::Rect xp_level_rect = {787, 657, 37, 16};

    void enqueue_message(const std::string& message, SDL_Color color);

    void cut_text_if_necessary(int& text_width, int max_width);

    void add_twinkling_bar(std::string& display_text);

    void render_text(const std::string& text, const SDL2pp::Rect& box_limit, SDL2pp::Font& font);

    void render_bar_value(const SDL2pp::Rect& box, const BarValue& value);

    void render_inventory();

    void render_equipment();

    void render_item(const SDL2pp::Rect& slot, uint8_t item_id);

    void render_item_amount(const SDL2pp::Rect& slot, uint8_t amount);

    void handle_chat_message(const ActionDTO& action);

    void handle_chat_list(const ActionDTO& action);

    void handle_list_bank(const ActionDTO& action);

    void handle_list_items(const ActionDTO& action);

    SDL_Color assign_message_color(const MessageType& type);

    bool is_receiver_or_sender(const MessageType& type);

    bool is_receiver(const MessageType& type);

    size_t get_visible_lines() const;

    SDL2pp::Texture& get_item_texture(uint8_t item_id);

public:
    UserInterface(SDL2pp::Renderer& renderer, std::string& player_name);

    void render();

    // TODO capaz conviene que el render lo haga todo solo, todavía no lo sé
    void render_fields();

    void render_chat_history();

    void render_chat_input(const std::string& input, bool is_chat_active);

    void update_player_state(const std::vector<PlayerInfoDTO>& players_information);

    void update_chat(const std::vector<ActionDTO>& actions);

    void chat_scroll_up();

    void chat_scroll_down();

    void chat_scroll_to_bottom();

    bool is_over_chat(const int x, const int y);

    int get_inventory_slot_at(int x, int y) const;

    std::optional<uint8_t> get_item_in_inventory_slot(int slot_index) const;

    void bind_item(int slot_index);

    std::optional<uint8_t> get_bound_item_id() const;

    void clear_bound_item();

    int get_equipment_slot_at(int x, int y) const;

    std::optional<uint8_t> get_item_in_equipment_slot(int slot_index) const;
};


#endif  // USER_INTERFACE_H
