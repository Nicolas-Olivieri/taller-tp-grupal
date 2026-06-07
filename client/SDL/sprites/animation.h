#ifndef ANIMATION_H
#define ANIMATION_H

#include <vector>

#include "SDL2pp/Rect.hh"
#include "SDL2pp/Renderer.hh"


class Animation {
private:
    const int delay;
    const int frame_amount;
    std::vector<SDL2pp::Rect> frames;

public:
    Animation(int frame_amount, int delay, int x_offset, int y_offset, int width, int height);

    SDL2pp::Rect next_frame(int iteration) const;

    SDL2pp::Rect get_first() const;
};


#endif
