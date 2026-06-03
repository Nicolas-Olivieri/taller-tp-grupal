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

public:
    explicit Ally(const Position& position);

    InteractResult interact(Player& player) override;  // bind

    Position get_position() const;

    virtual AllyType get_type() const = 0;

    virtual AllyExecuteResult execute(Player& player, const AllyActionPayload& payload) = 0;

    ~Ally() override = default;
};


#endif  // ALLY_H
