#ifndef LIST_ITEMS_COMMAND_H
#define LIST_ITEMS_COMMAND_H

#include <memory>
#include <string>

#include "server/command/cmd_results/ally_execute/list_outcomes.h"
#include "server/command/command.h"


class ListItemsCommand: public Command {
private:
    std::string player_name;
    std::unique_ptr<ListOutcome> result;

public:
    explicit ListItemsCommand(const std::string& player_name);

    void execute(GameWorld& world) override;

    void build_snapshot(SnapshotBuilder& builder) override;

private:
    void handle_bank_outcome(SnapshotBuilder& builder, const BankVaultOutcome& bank_outcome) const;

    void handle_vendor_outcome(SnapshotBuilder& builder, const VendorListOutcome& vendor_outcome) const;
};


#endif  // LIST_ITEMS_COMMAND_H
