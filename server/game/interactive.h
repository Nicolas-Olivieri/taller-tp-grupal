#ifndef INTERACTIVE_H
#define INTERACTIVE_H


// forward declaration
class Player;


class Interactive {
public:
    virtual bool interact(Player&) = 0;

    virtual ~Interactive() = default;
};


#endif  // INTERACTIVE_H
