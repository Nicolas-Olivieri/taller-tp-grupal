#ifndef CHATMESSAGE_COMMAND_H
#define CHATMESSAGE_COMMAND_H

#include <string>

#include "common/dto/snapshot/snapshot_builder.h"
#include "server/command/command.h"


class ChatMessageCommand: public Command {
private:
    std::string sender;
    std::string receiver;
    std::string content;

public:
    explicit ChatMessageCommand(const std::string& sender, const std::string& receiver,
                                const std::string& content);

    void execute(GameWorld& world) override;

    void build_snapshot(SnapshotBuilder& builder) override;
};


#endif  // CHATMESSAGE_COMMAND_H
