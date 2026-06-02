#ifndef ALLY_H
#define ALLY_H

#include "common/creature_type.h"
#include "server/game/interactive.h"
#include "server/game/position.h"

#include "ally_action.h"

// Forward declaration
class Player;


class Ally: public Interactive {
protected:
    const Position position;

public:
    explicit Ally(const Position& position);

    bool interact(Player& player) override;  // bind

    Position get_position() const;

    virtual AllyType get_type() const = 0;

    virtual void execute(Player& player, const AllyAction& action) = 0;

    ~Ally() override = default;
};


#endif  // ALLY_H
