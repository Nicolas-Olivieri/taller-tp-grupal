#ifndef CHAT_BOX_UI_H
#define CHAT_BOX_UI_H

#include <deque>
#include <string>
#include <unordered_map>
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
    const UserInterfaceData& ui_config;
    const ChatData& chat_config;

    InterfaceSprite ui;

    std::string player_name;
    std::string clan_name;

    std::deque<MsgData> chat_history;
    std::vector<TextSprite> visible_texts;

    SDL2pp::Color white;
    size_t first_visible_message = 0;

    TextSprite input_msg;

    std::unordered_map<MessageType, SDL_Color> msg_type_to_color;

    void init_texts();
    void init_color_msg();

    void render_chat_history();
    void render_chat_input(const std::string& input, bool is_chat_active);
    void add_twinkling_bar(std::string& display_text);

    void enqueue_message(const std::string& message, SDL2pp::Color color);

    void handle_chat_message(const ActionDTO& action);
    void handle_chat_list(const ActionDTO& action);
    void handle_list_bank(const ActionDTO& action);
    void handle_list_items(const ActionDTO& action);
    void handle_clan_message(const ActionDTO& action);

    SDL2pp::Color assign_message_color(const MessageType& type) const;
    size_t get_visible_lines() const;

public:
    ChatBoxUI(SpriteCreator& sprite_creator, const std::string& username);

    void render(const std::string& input, bool is_chat_active);

    void update_player_state(const PlayerInfoDTO& player);

    void update_chat(const std::vector<ActionDTO>& actions);

    void chat_scroll_up();

    void chat_scroll_down();

    void chat_scroll_to_bottom();

    bool is_over_chat(const int x, const int y);
};


#endif  // CHAT_BOX_UI_H
