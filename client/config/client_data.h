#ifndef CLIENT_DATA_H
#define CLIENT_DATA_H
#include <string>
#include <vector>

struct CreatureDisplayData {
    std::string name;
};

struct ItemDisplayData {
    // TODO: Agregar el resto de atributos de un ítem para el cliente
    std::string name;
    std::string icon_path;
};

struct RenderData {
    uint8_t fps;
    uint16_t screen_w;
    uint16_t screen_h;
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
    SDL2pp::Rect history_messages;
    SDL2pp::Rect input_box;

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
};

struct SoundData {
    uint8_t max_sound_distance;
    // TODO agregar el mapeo de id_weapon a sonido?
};

struct ChatData {
    uint8_t line_spacing;
    uint16_t max_chat_history;
};

#endif  // CLIENT_DATA_H
