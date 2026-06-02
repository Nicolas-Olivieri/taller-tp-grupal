#ifndef INTERACTIVE_H
#define INTERACTIVE_H


#include "server/command/cmd_results/interact/interact_result.h"
// forward declaration
class Player;


class Interactive {
public:
    virtual InteractResult interact(Player&) = 0;

    virtual ~Interactive() = default;
};


#endif  // INTERACTIVE_H
