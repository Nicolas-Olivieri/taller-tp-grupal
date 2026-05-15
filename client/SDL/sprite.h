#ifndef ENTITY_H
#define ENTITY_H
#include <functional>
#include <map>

#include "animation.h"
#include "SDL2pp/SDL2pp.hh"

enum Action {
    WALK_RIGHT,
    WALK_LEFT,
    WALK_UP,
    WALK_DOWN,
    IDLE
};

class Sprite {
private:
    SDL2pp::Point position;
    SDL2pp::Point size;

    bool idle;
    Action current_action;
    SDL2pp::Rect current_frame;

    std::map<Action, Animation> animations;
    SDL2pp::Texture& texture;
    SDL2pp::Renderer& renderer;

public:
    Sprite(SDL2pp::Renderer& renderer, SDL2pp::Texture& texture, int x, int y, int width, int height);

    void render() const;

    void update_frame(int iteration);

    void update(Action direction, const SDL2pp::Point &new_position);

    SDL2pp::Point get_position() const;
};


#endif
