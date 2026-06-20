#ifndef CLIENT_DATA_H
#define CLIENT_DATA_H
#include <cstdint>
#include <string>
#include <vector>

#include <SDL2pp/Rect.hh>

#include "SDL2pp/Color.hh"

struct CreatureDisplayData {
    std::string name;
};

struct ItemDisplayData {
    // TODO: Agregar el resto de atributos de un ítem para el cliente
    std::string name;
    std::string normalized_name;
    std::string icon_path;
};

struct RenderData {
    uint8_t fps;
    uint16_t tile_size;
};

struct SpriteData {
    uint8_t ghost_head_id;
    uint8_t ghost_body_id;
    int head_offset;
};

struct MovementData {
    uint8_t min_pixels_per_step;
    float change_rate;
};

struct UserInterfaceData {
    uint16_t window_width;
    uint16_t window_height;

    SDL2pp::Rect viewport;
    SDL2pp::Rect minimize_button;
    SDL2pp::Rect maximize_button;
    SDL2pp::Rect close_button;
    SDL2pp::Rect header_bar;

    SDL2pp::Rect chat_box;
    SDL2pp::Rect inventory;
    SDL2pp::Rect screen;

    SDL2pp::Rect history_messages;
    SDL2pp::Rect input_box;
    SDL2pp::Rect chat_icon;
    uint16_t chat_line_spacing;

    SDL2pp::Rect username;
    SDL2pp::Rect clan;
    SDL2pp::Rect founder;

    SDL2pp::Rect inventory_title;
    std::vector<SDL2pp::Rect> inventory_slots;

    std::vector<SDL2pp::Rect> equipment_slots;

    SDL2pp::Rect stats_title;
    SDL2pp::Rect health;
    SDL2pp::Rect mana;
    SDL2pp::Rect xp;
    SDL2pp::Rect safe_gold;
    SDL2pp::Rect excess_gold;
    SDL2pp::Rect xp_level;

    std::vector<SDL2pp::Rect> equipment_state_slots;
};

struct ColorData {
    SDL2pp::Color yellow;
    SDL2pp::Color grey;
    SDL2pp::Color white;
    SDL2pp::Color green;
    SDL2pp::Color red;
    SDL2pp::Color light_blue;
    SDL2pp::Color black;
};

struct SoundData {
    uint8_t max_sound_distance;
    // TODO agregar el mapeo de id_weapon a sonido?
};

struct ChatData {
    uint16_t max_chat_history;
};

#endif  // CLIENT_DATA_H
