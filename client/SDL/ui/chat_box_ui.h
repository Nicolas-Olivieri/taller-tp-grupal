#ifndef CHAT_BOX_UI_H
#define CHAT_BOX_UI_H

#include <deque>
#include <string>
#include <vector>

#include "client/SDL/sprite_creation/sprite_creator.h"
#include "client/config/client_data.h"

struct MsgData {
    std::string text;
    SDL2pp::Color color;
};

class ChatBoxUI {
private:
    SpriteCreator& creator;
    const UserInterfaceData& config;
    const ChatData& chat_config;

    InterfaceSprite ui;

    std::string player_name;
    std::string clan_name;

    std::deque<MsgData> chat_history;
    std::vector<TextSprite> visible_texts;

    SDL2pp::Color white = {255, 255, 255, 255};
    size_t first_visible_message = 0;

    TextSprite input_msg;

    SDL2pp::Color yellow = {235, 224, 70, 255};
    SDL2pp::Color grey = {255, 255, 255, 140};
    SDL2pp::Color green = {44, 230, 66, 140};
    SDL2pp::Color red = {214, 30, 30, 255};
    SDL2pp::Color light_blue = {44, 172, 230, 140};

    void init_texts();

    void render_chat_history();
    void render_chat_input(const std::string& input, bool is_chat_active);
    void add_twinkling_bar(std::string& display_text);

    void enqueue_message(const std::string& message, SDL2pp::Color color);

    void handle_chat_message(const ActionDTO& action);
    void handle_chat_list(const ActionDTO& action);
    void handle_list_bank(const ActionDTO& action);
    void handle_list_items(const ActionDTO& action);
    void handle_clan_message(const ActionDTO& action);

    SDL2pp::Color assign_message_color(const MessageType& type);
    size_t get_visible_lines() const;

public:
    ChatBoxUI(SpriteCreator& sprite_creator, const std::string& username);

    void render(const std::string& input, bool is_chat_active);

    void update_chat(const std::vector<ActionDTO>& actions);

    void chat_scroll_up();

    void chat_scroll_down();

    void chat_scroll_to_bottom();

    bool is_over_chat(const int x, const int y);
};


#endif  // CHAT_BOX_UI_H
