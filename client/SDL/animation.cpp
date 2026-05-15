#include "animation.h"

Animation::Animation(const int frame_amount, const int x_offset, const int y_offset, const int height, const int width) :
    frame_amount(frame_amount)
{
    for (int i = 0; i < frame_amount; i++) {
        const SDL2pp::Rect frame(x_offset + i*width, y_offset, width, height);
        frames.push_back(frame);
    }
}

SDL2pp::Rect Animation::next_frame(const int iteration) const {
    return frames[iteration % frames.size()];
}
