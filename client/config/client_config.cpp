#include "client_config.h"

#include <algorithm>
#include <string>

#include <toml.hpp>

#include "common/util/string_utils.h"

#define CLIENT_ITEMS_PATH "/client/items.toml"
#define CLIENT_CREATURES_PATH "/client/creatures.toml"
#define CLIENT_CONSTANTS_PATH "/client/game_constants.toml"
#define CLIENT_UI_DATA_PATH "/client/user_interface"
#define CLIENT_UI_DATA_PATH_DEFAULT "/client/user_interface_1920x1080.toml"
#define CLIENT_SOUND_DATA_PATH "/client/sound_data.toml"
#define CLIENT_CHAT_DATA_PATH "/client/chat.toml"
#define CLIENT_COLOR_PATH "/client/color.toml"
#define CLIENT_HELP_DATA_PATH "/client/help.toml"


ClientConfig::ClientConfig() {
    load_items_data(toml::parse(CONFIG_PATH CLIENT_ITEMS_PATH));
    load_creatures_data(toml::parse(CONFIG_PATH CLIENT_CREATURES_PATH));
    load_constants_data(toml::parse(CONFIG_PATH CLIENT_CONSTANTS_PATH));
    load_ui_data(toml::parse(CONFIG_PATH CLIENT_UI_DATA_PATH_DEFAULT));
    load_sound_data(toml::parse(CONFIG_PATH CLIENT_SOUND_DATA_PATH));
    load_chat_data(toml::parse(CONFIG_PATH CLIENT_CHAT_DATA_PATH));
    load_color_data(toml::parse(CONFIG_PATH CLIENT_COLOR_PATH));
    load_help_msg_data(toml::parse(CONFIG_PATH CLIENT_HELP_DATA_PATH));
}


ClientConfig& ClientConfig::get() {
    static ClientConfig instance;
    return instance;
}


const CreatureDisplayData& ClientConfig::get_creature_data(const uint8_t creature) const {
    if (not creatures_data.contains(creature)) {
        throw std::out_of_range(
                "ClientConfig recibió una criatura que no se encuentra en la configuración del cliente: " +
                std::to_string(creature));
    }

    return creatures_data.at(creature);
}


std::string ClientConfig::get_creature_name(const uint8_t creature) const {
    // Si llega un ID que el cliente no conoce, se muestra un error amigable en lugar de estallar
    if (not creatures_data.contains(creature)) {
        return "???";
    }

    return creatures_data.at(creature).name;
}


const ItemDisplayData& ClientConfig::get_item_data(const uint8_t item_id) const {
    if (not items_data.contains(item_id)) {
        throw std::out_of_range(
                "ClientConfig recibió un ítem que no se encuentra en la configuración del cliente: " +
                std::to_string(item_id));
    }

    return items_data.at(item_id);
}


std::string ClientConfig::get_item_name(const uint8_t item_id) const {
    // Si llega un ID que el cliente no conoce, se muestra un error amigable en el chat en lugar de estallar
    if (not items_data.contains(item_id)) {
        return "Objeto desconocido (" + std::to_string(item_id) + ")";
    }

    return items_data.at(item_id).name;
}


std::optional<uint8_t> ClientConfig::get_item_id(const std::string& item_name) const {
    const std::string target_name = string_utils::normalize(item_name);
    for (const auto& [id, data]: items_data) {
        if (data.normalized_name == target_name)
            return id;
    }

    return std::nullopt;
}


std::string ClientConfig::get_item_icon_path(const uint8_t item_id) {
    if (not items_data.contains(item_id)) {
        return "resources/items/0.png";  // TODO: Agregar una textura de error
    }

    return items_data.at(item_id).icon_path;
}

ItemDisplayData ClientConfig::build_item_display_data(const toml::value& item_toml) const {
    const auto name = toml::find<std::string>(item_toml, "name");
    return ItemDisplayData(name, string_utils::normalize(name),
                           toml::find<std::string>(item_toml, "icon_path"));
}

CreatureDisplayData ClientConfig::build_creature_display_data(const toml::value& creature_toml) const {
    return CreatureDisplayData(toml::find<std::string>(creature_toml, "name"));
}

uint8_t ClientConfig::get_ghost_head_id() const { return sprite_data.ghost_head_id; }

uint8_t ClientConfig::get_ghost_body_id() const { return sprite_data.ghost_body_id; }

int ClientConfig::get_head_offset() const { return sprite_data.head_offset; }

uint8_t ClientConfig::get_fps() const { return render_data.fps; }

uint16_t ClientConfig::get_screen_w() const { return ui_data.window_width; }

uint16_t ClientConfig::get_screen_h() const { return ui_data.window_height; }

uint16_t ClientConfig::get_tile_size() const { return render_data.tile_size; }

const MovementData& ClientConfig::get_movement_data() const { return movement_data; }

SDL2pp::Rect ClientConfig::parse_rect(const toml::value& config, const std::string& section,
                                      const std::string& key) {
    auto values = toml::find<std::vector<int>>(config, section, key);

    return SDL2pp::Rect(values[0], values[1], values[2], values[3]);
}

std::vector<SDL2pp::Rect> ClientConfig::parse_rect_vector(const toml::value& config,
                                                          const std::string& section,
                                                          const std::string& key) {
    auto rects = toml::find<std::vector<std::vector<int>>>(config, section, key);

    std::vector<SDL2pp::Rect> result;
    result.reserve(rects.size());

    std::transform(rects.begin(), rects.end(), std::back_inserter(result), [](const std::vector<int>& rect) {
        return SDL2pp::Rect(rect[0], rect[1], rect[2], rect[3]);
    });

    return result;
}

const UserInterfaceData& ClientConfig::get_ui_data() const { return ui_data; }

const SoundData& ClientConfig::get_sound_data() const { return sound_data; }


const ChatData& ClientConfig::get_chat_data() const { return chat_data; }

void ClientConfig::load_items_data(toml::basic_value<toml::type_config> root) {
    const auto items_table = toml::find(root, "items");

    for (const auto& [key, value]: items_table.as_table()) {
        uint8_t id = static_cast<uint8_t>(toml::find<int>(value, "id"));
        items_data[id] = build_item_display_data(value);
    }
}

void ClientConfig::load_creatures_data(toml::basic_value<toml::type_config> root) {
    const auto creatures_table = toml::find(root, "creatures");

    for (const auto& [key, value]: creatures_table.as_table()) {
        uint8_t id = static_cast<uint8_t>(toml::find<int>(value, "id"));
        creatures_data[id] = build_creature_display_data(value);
    }
}

void ClientConfig::load_constants_data(toml::basic_value<toml::type_config> root) {
    const auto constants_table = toml::find(root, "constants");

    for (const auto& [key, value]: constants_table.as_table()) {
        if (key == "render") {
            render_data = {
                    toml::find<uint8_t>(value, "fps"),
                    toml::find<uint16_t>(value, "tile_size"),
            };
            continue;
        }

        if (key == "sprites") {
            sprite_data = {
                    toml::find<uint8_t>(value, "ghost_head_id"),
                    toml::find<uint8_t>(value, "ghost_body_id"),
                    toml::find<int>(value, "head_offset"),
            };
            continue;
        }

        if (key == "movement") {
            movement_data = {toml::find<uint8_t>(value, "min_pixels_per_step"),
                             toml::find<float>(value, "change_rate")};
            continue;
        }

        throw std::runtime_error(
                std::format("ClientConfig encontró un tipo de constante desconocido: {}", key));
    }
}

const UserInterfaceData& ClientConfig::load_resolution_data(const std::string& resolution) {
    const std::string path = std::format("{}_{}.toml", DATA_PATH CLIENT_UI_DATA_PATH, resolution);
    if (path == DATA_PATH CLIENT_UI_DATA_PATH_DEFAULT) {
        return ui_data;
    }
    load_ui_data(toml::parse(path));
    return ui_data;
}


void ClientConfig::load_ui_data(toml::basic_value<toml::type_config> root) {
    const auto ui_table = toml::find(root, "ui");

    ui_data.window_width = toml::find<int>(ui_table, "resolution", "window_width");
    ui_data.window_height = toml::find<int>(ui_table, "resolution", "window_height");

    ui_data.viewport = parse_rect(ui_table, "layout", "viewport");
    ui_data.minimize_button = parse_rect(ui_table, "layout", "minimize_button");
    ui_data.maximize_button = parse_rect(ui_table, "layout", "maximize_button");
    ui_data.close_button = parse_rect(ui_table, "layout", "close_button");
    ui_data.header_bar = parse_rect(ui_table, "layout", "header_bar");

    ui_data.history_messages = parse_rect(ui_table, "chat", "history_messages");
    ui_data.input_box = parse_rect(ui_table, "chat", "input_box");
    ui_data.chat_icon = parse_rect(ui_table, "chat", "chat_icon");
    ui_data.chat_line_spacing = toml::find<uint8_t>(ui_table, "chat", "chat_line_spacing"),

    ui_data.username = parse_rect(ui_table, "player", "username");
    ui_data.clan = parse_rect(ui_table, "player", "clan");
    ui_data.founder = parse_rect(ui_table, "player", "founder_icon");

    ui_data.inventory_title = parse_rect(ui_table, "inventory", "title");
    ui_data.inventory_slots = parse_rect_vector(ui_table, "inventory", "slots");

    ui_data.equipment_slots = parse_rect_vector(ui_table, "equipment", "slots");

    ui_data.stats_title = parse_rect(ui_table, "stats", "title");
    ui_data.health = parse_rect(ui_table, "stats", "health_bar");
    ui_data.mana = parse_rect(ui_table, "stats", "mana_bar");
    ui_data.xp = parse_rect(ui_table, "stats", "xp_bar");

    ui_data.safe_gold = parse_rect(ui_table, "stats", "safe_gold");
    ui_data.excess_gold = parse_rect(ui_table, "stats", "excess_gold");
    ui_data.xp_level = parse_rect(ui_table, "stats", "xp_level");

    ui_data.equipment_state_slots = parse_rect_vector(ui_table, "stats", "equipment_state_slots");

    ui_data.chat_box = parse_rect(ui_table, "layout", "chat_box");
    ui_data.inventory = parse_rect(ui_table, "layout", "inventory");
    ui_data.screen = parse_rect(ui_table, "layout", "screen");
}

void ClientConfig::load_sound_data(toml::basic_value<toml::type_config> root) {
    const auto sound_table = toml::find(root, "sound");

    sound_data.max_sound_distance = toml::find<uint8_t>(root, "sound", "distance", "max_sound_distance");

    const auto& weapons_array = toml::find(root, "sound", "weapons").as_array();
    sound_data.weapon_to_sound_event.clear();

    assert(sound_data.weapon_to_sound_event.empty());
    for (const auto& entry: weapons_array) {
        uint8_t weapon_id = toml::find<uint8_t>(entry, "id");
        SoundEvent sound_event = static_cast<SoundEvent>(toml::find<int>(entry, "event"));

        sound_data.weapon_to_sound_event.insert({weapon_id, sound_event});
    }
}

void ClientConfig::load_chat_data(toml::basic_value<toml::type_config> root) {
    const auto chat_table = toml::find(root, "chat");

    for (const auto& [key, value]: chat_table.as_table()) {
        if (key == "data") {
            chat_data = {toml::find<uint16_t>(value, "max_chat_history"),
                         toml::find<uint16_t>(value, "ms_between_cursor_appearance")};

            continue;
        }

        throw std::runtime_error(std::format("ClientConfig encontró un dato del chat desconocido: {}", key));
    }
}

void ClientConfig::load_color_data(toml::basic_value<toml::type_config> root) {
    const auto color_table = toml::find(root, "color");

    color_data.yellow = to_color(parse_rect(color_table, "colors", "yellow"));
    color_data.grey = to_color(parse_rect(color_table, "colors", "grey"));
    color_data.white = to_color(parse_rect(color_table, "colors", "white"));
    color_data.green = to_color(parse_rect(color_table, "colors", "green"));
    color_data.red = to_color(parse_rect(color_table, "colors", "red"));
    color_data.light_blue = to_color(parse_rect(color_table, "colors", "light_blue"));
    color_data.black = to_color(parse_rect(color_table, "colors", "black"));
    color_data.transparent_black = to_color(parse_rect(color_table, "colors", "transparent_black"));
}

const ColorData& ClientConfig::get_color_data() const { return color_data; }

SDL2pp::Color ClientConfig::to_color(const SDL2pp::Rect& rect) {
    return SDL2pp::Color(rect.x, rect.y, rect.w, rect.h);
}

const HelpMsgData& ClientConfig::get_help_data() const { return help_data; }

void ClientConfig::load_help_msg_data(toml::basic_value<toml::type_config> root) {
    const auto help_table = toml::find(root, "help");

    help_data.start_help = toml::find<std::string>(help_table, "general", "start_help");
    help_data.help_messages = toml::find<std::vector<std::string>>(help_table, "general", "help_messages");
    help_data.top_margin = toml::find<uint16_t>(help_table, "spaces", "top_margin");
    help_data.left_margin = toml::find<uint16_t>(help_table, "spaces", "left_margin");
    help_data.line_spacing = toml::find<uint16_t>(help_table, "spaces", "line_spacing");
    help_data.max_msg_per_page = toml::find<uint16_t>(help_table, "spaces", "max_msg_per_page");
    help_data.clan_help_messages = toml::find<std::vector<std::string>>(help_table, "clan", "help_messages");
    help_data.cheat_help_messages =
            toml::find<std::vector<std::string>>(help_table, "cheat", "help_messages");
    help_data.npc_help_messages = toml::find<std::vector<std::string>>(help_table, "npc", "help_messages");
}
