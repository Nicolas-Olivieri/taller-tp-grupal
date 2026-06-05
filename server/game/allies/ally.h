#ifndef ALLY_H
#define ALLY_H

#include "common/npc_type.h"
#include "server/command/cmd_results/ally_execute/ally_execute_result.h"
#include "server/game/interactive.h"
#include "server/game/position.h"

#include "ally_action_payload.h"


// Forward declaration
class Player;


class Ally: public Interactive {
protected:
    const Position position;
    AllyType type;

    template <typename TResult, typename TStatus>
    AllyExecuteResult handle_action_not_accepted(TStatus not_accepted_status) const {
        return AllyExecuteResult(TResult(not_accepted_status, type));
    }

public:
    explicit Ally(const Position& position, const AllyType& type);

    InteractResult interact(Player& player) override;  // bind

    Position get_position() const;

    AllyType get_type() const;

    virtual AllyExecuteResult execute(Player& player, const AllyActionPayload& payload) const = 0;

    ~Ally() override = default;
};


#endif  // ALLY_H
