#ifndef MOVING_SPRITE_H
#define MOVING_SPRITE_H
#include "sprite.h"
#include "common/direction.h"


class MovingSprite : public Sprite {
protected:
    SDL2pp::Point target_position;
    Direction direction;

    int get_new_coordinate(const int &current_coordinate, const int &coordinate_diff);

    MovingSprite(SDL2pp::Point position, SDL2pp::Point size, Direction direction);

    bool is_idle() const;

public:
    void update_visual_position();

    void set_target_position(Direction new_direction, const SDL2pp::Point& new_target);

    SDL2pp::Point get_target_position() const;

    Direction get_direction() const;

    ~MovingSprite() override = default;

};


#endif //MOVING_SPRITE_H
