#include "chatmessage_command.h"

ChatMessageCommand::ChatMessageCommand(const std::string& sender, const std::string& receiver,
                                       const std::string& content):
        sender(sender), receiver(receiver), content(content) {}

void ChatMessageCommand::execute(GameWorld& /* world */) {}

void ChatMessageCommand::build_snapshot(SnapshotBuilder& builder) {
    MessageType type = receiver.empty() ? MessageType::GLOBAL : MessageType::PRIVATE;

    builder.add_action(ActionDTO(ChatMessageDTO(type, sender, receiver, content)));
}
