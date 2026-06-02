#include "chatmessage_command.h"

ChatMessageCommand::ChatMessageCommand(const std::string& sender, const std::string& receiver,
                                       const std::string& content):
        sender(sender), receiver(receiver), content(content) {}

void ChatMessageCommand::execute(GameWorld& /* world */) {
    // TODO: honestamente, a lo sumo podría tener que chequear si el receiver del mensaje es un jugador o un
    // clan para cambiar la visibility en el build_snapshot
}

void ChatMessageCommand::build_snapshot(SnapshotBuilder& builder) {
    builder.add_action(ActionDTO(ChatMessageDTO(MessageVisibility::PRIVATE, sender, receiver, content)));
}
